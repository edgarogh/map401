use std::ops::{Add, Mul};

pub trait ApproxEq {
    fn approx_eq(self, rhs: Self) -> bool;
}

impl ApproxEq for f64 {
    fn approx_eq(self, rhs: Self) -> bool {
        (self - rhs).abs() < f64::EPSILON
    }
}

impl ApproxEq for Vec2 {
    fn approx_eq(self, rhs: Self) -> bool {
        (self.0.approx_eq(rhs.0)) && (self.1.approx_eq(rhs.1))
    }
}

#[derive(Clone, Copy, Debug)]
#[repr(C)]
pub struct Vec2(pub f64, pub f64);

impl Vec2 {
    pub fn from_points(a: Vec2, b: Vec2) -> Self {
        Vec2(b.0 - a.0, b.1 - a.1)
    }

    pub fn magnitude(&self) -> f64 {
        (self.0.powf(2.) + self.1.powf(2.)).sqrt()
    }
}

impl Add for Vec2 {
    type Output = Vec2;

    fn add(self, rhs: Self) -> Self::Output {
        Vec2(self.0 + rhs.0, self.1 + rhs.1)
    }
}

impl Mul<f64> for Vec2 {
    type Output = Vec2;

    fn mul(self, rhs: f64) -> Self::Output {
        Vec2(self.0 * rhs, self.1 * rhs)
    }
}

/// Dot product
impl Mul for Vec2 {
    type Output = f64;

    fn mul(self, rhs: Self) -> Self::Output {
        (self.0 * rhs.0) + (self.1 * rhs.1)
    }
}

// C FFI

use std::os::raw::{c_double};

#[no_mangle]
pub extern "C" fn egaux_doubles(a: c_double, b: c_double) -> bool {
    a.approx_eq(b)
}

#[no_mangle]
pub extern "C" fn set_point(a: f64, b: f64) -> Vec2 {
    Vec2(a, b)
}

#[no_mangle]
pub extern "C" fn add_point(a: Vec2, b: Vec2) -> Vec2 {
    a + b
}

#[no_mangle]
pub extern "C" fn mul_reel_point(a: Vec2, b: c_double) -> Vec2 {
    a * b
}

#[no_mangle]
pub extern "C" fn distance(a: Vec2, b: Vec2) -> c_double {
    Vec2::from_points(a, b).magnitude()
}

#[no_mangle]
pub extern "C" fn egaux_points(a: Vec2, b: Vec2) -> bool {
    a.approx_eq(b)
}

#[no_mangle]
pub extern "C" fn set_vecteur(a: c_double, b: c_double) -> Vec2 {
    Vec2(a, b)
}

#[no_mangle]
pub extern "C" fn add_vecteur(a: Vec2, b: Vec2) -> Vec2 {
    a + b
}

#[no_mangle]
pub extern "C" fn mul_reel_vecteur(a: Vec2, b: c_double) -> Vec2 {
    a * b
}

#[no_mangle]
pub extern "C" fn vect_bipoint(a: Vec2, b: Vec2) -> Vec2 {
    Vec2::from_points(a, b)
}

#[no_mangle]
pub extern "C" fn produit_scalaire(a: Vec2, b: Vec2) -> f64 {
    a * b
}

#[no_mangle]
pub extern "C" fn egaux_vecteurs(a: Vec2, b: Vec2) -> bool {
    a.approx_eq(b)
}

#[no_mangle]
pub extern "C" fn norme(v: Vec2) -> c_double {
    v.magnitude()
}
