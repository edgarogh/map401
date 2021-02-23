use std::os::raw::{c_uint, c_char, c_double, c_int};
use std::ffi::{CString, CStr, NulError};
use std::num::NonZeroU32;
use std::ptr::NonNull;
use std::io::{Error as IoError, ErrorKind as IoErrorKind, Result as IoResult};

#[inline]
pub fn nul_io_error() -> IoError {
    IoError::new(IoErrorKind::NotFound, "unexpected NUL character in string")
}

/// C File
#[repr(C)]
struct CFile([u8; 0]);
#[repr(transparent)]
pub struct CFileRef(NonNull<CFile>);

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
#[repr(C)]
pub enum Pixel {
    BLANC = 0,
    NOIR = 1,
}

#[repr(C)]
pub struct Image {
    width: c_uint,
    height: c_uint,
    inner: *mut Pixel,
}

#[repr(transparent)]
struct Mask(Image);

#[repr(C)]
struct ContourLinkedNode {
    value: (c_double, c_double),
    next: Option<Box<ContourLinkedNode>>,
}

#[repr(C)]
struct ContourLinked {
    len: c_uint,
    first: Option<Box<ContourLinkedNode>>,
    last: *mut ContourLinkedNode,
}

struct ContourTabInner;

#[repr(C)]
struct ContourTab {
    len: c_uint,
    inner: Box<ContourTabInner>,
}

#[derive(Clone, Copy)]
#[repr(C)]
pub enum OutMode {
    Stroked,
    StrokedPoints,
    Filled,
}

impl OutMode {
    pub const fn ext(self) -> &'static str {
        match self {
            Self::Stroked => "-mode1.eps",
            Self::StrokedPoints => "-mode2.eps",
            Self::Filled => "-mode3.eps",
        }
    }
}

#[repr(C)]
pub struct OutFile {
    file: NonNull<CFile>,
    mode: OutMode,
    h: c_uint,
}

extern {
    fn fopen(path: *const c_char, mode: *const c_char) -> *mut CFile;
    fn fputs(str: *const c_char, self_: NonNull<CFile>) -> c_int;
    fn fclose(self_: NonNull<CFile>);

    fn lire_fichier_image(path: *const c_char) -> Image;
    fn supprimer_image(image: &mut Image);

    fn liste_points_to_tableau_points(self_: ContourLinked) -> ContourTab;
    // fn liste_points_supprimer(self_: &mut ContourLinked);
    // fn tableau_points_supprimer(self_: &mut ContourTab);
    fn tableau_points_enregistrer(self_: &ContourTab, file: NonNull<CFile>);

    fn contour_init_mask(image: Image) -> Mask;
    fn contour(image: Image, mut_mask: Mask) -> ContourLinked;

    fn sortie_open(path: *const c_char, width: c_uint, height: c_uint, mode: OutMode) -> OutFile;
    fn sortie_ecrire_contour(self_: OutFile, contour: ContourTab);
    fn sortie_close(self_: OutFile);
}

impl CFileRef {
    pub fn open_write(path: impl Into<Vec<u8>>) -> IoResult<CFileRef> {
        let path = CString::new(path).map_err(|_| nul_io_error())?;

        unsafe {
            let pointer = NonNull::new(fopen(
                path.as_c_str().as_ptr(),
                CStr::from_bytes_with_nul_unchecked(&[b'w', 0]).as_ptr(),
            ));

            Ok(CFileRef(pointer.ok_or(std::io::Error::last_os_error())?))
        }
    }

    pub fn write(&mut self, text: impl Into<Vec<u8>>) -> Result<(), NulError> {
        unsafe {
            fputs(CString::new(text)?.as_ptr(), self.0);
        }

        Ok(())
    }
}

impl Drop for CFileRef {
    fn drop(&mut self) {
        unsafe { fclose(self.0) };
    }
}

impl Image {
    pub fn from_raw_components(width: c_uint, height: c_uint, inner: *mut Pixel) -> Self {
        Self {
            width,
            height,
            inner,
        }
    }

    pub fn open(path: impl Into<Vec<u8>>) -> IoResult<Self> {
        let path = CString::new(path).map_err(|_| nul_io_error())?;
        Ok(unsafe { lire_fichier_image(path.as_ptr()) })
    }

    pub fn width(&self) -> u32 {
        self.width
    }

    pub fn height(&self) -> u32 {
        self.height
    }
}

impl Drop for Image {
    fn drop(&mut self) {
        unsafe { supprimer_image(self) };
    }
}

impl From<&Image> for Mask {
    fn from(i: &Image) -> Self {
        unsafe {
            contour_init_mask(std::ptr::read(i))
        }
    }
}

impl From<&ContourLinked> for ContourTab {
    fn from(contour: &ContourLinked) -> Self {
        unsafe { liste_points_to_tableau_points(std::ptr::read(contour)) }
    }
}

pub struct Contour {
    len: NonZeroU32,
    inner: Box<ContourTabInner>,
}

impl Contour {
    fn new(contour: impl Into<ContourTab>) -> Option<Self> {
        let contour = contour.into();

        Some(Self {
            len: NonZeroU32::new(contour.len)?,
            inner: contour.inner,
        })
    }
}

impl Contour {
    pub fn len(&self) -> usize {
        self.len.get() as _
    }
}

impl From<Contour> for ContourTab {
    fn from(contour: Contour) -> Self {
        Self {
            len: contour.len.get(),
            inner: contour.inner,
        }
    }
}

pub fn write_contour(contour: &Contour, file: &CFileRef) {
    unsafe {
        let contour = std::ptr::read(contour).into();
        tableau_points_enregistrer(&contour, file.0);
        std::mem::forget(contour);
    }
}

impl OutFile {
    pub fn open(path: impl Into<Vec<u8>>, width: u32, height: u32, mode: OutMode) -> IoResult<Self> {
        let path = CString::new(path).map_err(|_| nul_io_error())?;
        Ok(unsafe { sortie_open(path.as_ptr(), width, height, mode) })
    }

    pub fn write(&mut self, contour: &Contour) {
        unsafe { sortie_ecrire_contour(std::ptr::read(self), std::ptr::read(contour).into()) };
    }
}

impl Drop for OutFile {
    fn drop(&mut self) {
        unsafe { sortie_close(std::ptr::read(self)) };
    }
}

pub struct ContourExtractorOnTheFly<'a> {
    image: &'a Image,
    pub(self) mask: Mask,
}

/// By default, the contour extractor extracts contours "on the fly".
/// However, because knowing the length of the contour iterator list is impossible before iterating
/// over every contour, calling [ContourExtractor::len][ContourExtractor::len] will collect all the
/// contours into a `Vec` and mutate the [ContourExtractor][ContourExtractor] in the `Precomputed`
/// variant.
pub enum ContourExtractor<'a> {
    OnTheFly(ContourExtractorOnTheFly<'a>),
    Precomputed(std::vec::IntoIter<Contour>),
}

impl<'a> ContourExtractor<'a> {
    pub fn len(&mut self) -> usize {
        let (new_self, len) = match &self {
            Self::Precomputed(into_iter) => {
                return into_iter.len()
            }
            _ => {
                let vec: Vec<_> = self.collect();
                let len = vec.len();
                (Self::Precomputed(vec.into_iter()), len)
            }
        };

        *self = new_self;

        len
    }
}

impl<'a> From<&'a Image> for ContourExtractor<'a> {
    fn from(image: &'a Image) -> Self {
        let mask = Mask::from(image);

        Self::OnTheFly(ContourExtractorOnTheFly {
            image,
            mask,
        })
    }
}

impl<'a> Iterator for ContourExtractor<'a> {
    type Item = Contour;

    fn next(&mut self) -> Option<Self::Item> {
        match self {
            Self::OnTheFly(ContourExtractorOnTheFly { image, mask }) => {
                let contour = unsafe { contour(
                    std::ptr::read(*image),
                    std::ptr::read(mask),
                ) };

                Contour::new(&contour)
            }
            Self::Precomputed(iter) => iter.next(),
        }
    }
}
