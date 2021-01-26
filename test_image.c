#include <assert.h>
#include "contour.h"
#include "image.h"

int main() {
    Image I = lire_fichier_image("images/caractere2.pbm");
    assert(I.L == 7);
    assert(I.H == 9);
    assert(get_pixel_image(I, 2, 2) == BLANC);
    assert(get_pixel_image(I, 3, 2) == NOIR);
    assert(get_pixel_image(I, 2, 3) == NOIR);
    ecrire_image(I);

    Contour c = contour(I);

    liste_points_ecrire(c);

    printf("\e[32mtest_image passé avec succès !\e[0m\n");
}
