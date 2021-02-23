fn main() {
    cc::Build::new()
        .extra_warnings(false)
        .file("../image.c")
        .file("../liste_points.c")
        .file("../contour.c")
        .file("../sortie_eps.c")
        .compile("contour_of.a");
}
