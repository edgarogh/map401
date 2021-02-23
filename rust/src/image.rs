use image_crate::ImageDecoder;
use image_crate::codecs::pnm::{PnmDecoder, PnmSubtype};
use std::mem::ManuallyDrop;
use std::fmt::Formatter;
use std::marker::PhantomData;
use crate::raw::Image as RawImage;
use std::ops::{Deref, DerefMut};

type Pixel = crate::raw::Pixel;

pub struct Image {
    width: usize,
    height: usize,
    inner: Box<[Pixel]>,
}

pub struct RawImageRef<'a> {
    image: ManuallyDrop<RawImage>,
    _phantom: PhantomData<&'a Image>,
}

impl<'a> Deref for RawImageRef<'a> {
    type Target = RawImage;

    fn deref(&self) -> &Self::Target {
        &*self.image
    }
}

impl<'a> DerefMut for RawImageRef<'a> {
    fn deref_mut(&mut self) -> &mut Self::Target {
        &mut *self.image
    }
}

impl Image {
    pub fn read(reader: impl std::io::Read) -> Self {
        let img = PnmDecoder::new(reader).unwrap();
        let (width, height) = img.dimensions();
        let (width, height) = (width as usize, height as usize);

        Self {
            width,
            height,
            inner: {
                let mut buffer = vec![0; width * height];
                img.read_image(&mut buffer).unwrap();
                buffer.into_iter().map(|byte| match byte {
                    0 => Pixel::BLANC,
                    1 | 255 => Pixel::NOIR,
                    _ => unimplemented!(),
                }).collect()
            },
        }
    }

    pub unsafe fn as_raw_image(&mut self) -> RawImageRef {
        RawImageRef {
            image: ManuallyDrop::new(crate::raw::Image::from_raw_components(
                self.width as _,
                self.height as _,
                self.inner.as_mut_ptr(),
            )),
            _phantom: PhantomData,
        }
    }
}

impl std::fmt::Debug for Image {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        // TODO format using conventional style & indentation
        writeln!(f, "{}x{}", self.width, self.height)?;
        for y in 0..self.height {
            let row: String = self.inner[(y * self.width)..((y + 1) * self.width)].iter().map(|pixel| match pixel {
                Pixel::BLANC => '#',
                Pixel::NOIR => ' ',
            }).collect();

            writeln!(f, "|{}|", row)?;
        }
        Ok(())
    }
}
