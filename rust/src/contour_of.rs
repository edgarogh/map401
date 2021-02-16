#![feature(array_map, try_trait)]

mod raw;

use raw::{Image, OutFile};
use clap::Clap;
use raw::{ContourExtractor, OutMode, write_contour, CFileRef};
use std::ffi::CString;

#[derive(Clap)]
struct Args {
    /// Path to the PBM image
    image: String,

    /// Should a contour file be generated
    #[clap(short = 'c')]
    generate_contours: bool,

    /// Should an EPS (mode 1) file be generated
    #[clap(short = '1')]
    generate_mode1: bool,

    /// Should an EPS (mode 2) file be generated
    #[clap(short = '2')]
    generate_mode2: bool,

    /// Should an EPS (mode 3) file be generated
    #[clap(short = '3')]
    generate_mode3: bool,
}

fn revert_opt_res<T, E>(opt: Option<Result<T, E>>) -> Result<Option<T>, E> {
    match opt {
        Some(Ok(ok)) => Ok(Some(ok)),
        Some(Err(err)) => Err(err),
        None => Ok(None),
    }
}

fn main() -> Result<(), u32> {
    match main_result() {
        Ok(()) => Ok(()),
        Err(msg) => {
            eprintln!("error: {}", msg);
            Err(1)
        }
    }
}

const CONTAINS_NUL: &str = "path contains NUL";

fn main_result() -> Result<(), &'static str> {
    let args: Args = Args::parse();
    // let image_path = CString::new(args.image).map_err(|_| "path contains NUL")?;

    let mut contours_file = match Some(())
        .filter(|_| args.generate_contours)
        .map(|_| CFileRef::open_write(format!("{}.contours", &args.image))) {
        Some(Err(_)) => return Err(CONTAINS_NUL),
        Some(Ok(f)) => Some(f),
        None => None,
    };

    let eps_files = [
        (args.generate_mode1, OutMode::Stroked),
        (args.generate_mode2, OutMode::StrokedPoints),
        (args.generate_mode3, OutMode::Filled),
    ]
        .map(|(enabled, mode)| Some(()).filter(|_| enabled).map(|_| (format!("{}{}", &args.image, mode.ext()), mode)));

    let image = Image::open(args.image).map_err(|_| CONTAINS_NUL)?;

    let eps_files = eps_files
        .map(|file| revert_opt_res(
            file.map(|(file, mode)| OutFile::open(
                file,
                image.width(),
                image.height(),
                mode,
            ))
        ));

    let mut eps_files = match eps_files {
        [Ok(a), Ok(b), Ok(c)] => [a, b, c],
        _ => return Err(CONTAINS_NUL),
    };

    let mut contours = ContourExtractor::from(&image);

    if let Some(contours_file) = &mut contours_file {
        contours_file.write(format!("{}\n\n", contours.len()));
    }

    for contour in contours {
        if let Some(contours_file) = &mut contours_file {
            write_contour(&contour, contours_file);
        }

        for eps in eps_files.iter_mut().filter_map(|o| o.as_mut()) {
            eps.write(&contour);
        }
    }

    Ok(())
}
