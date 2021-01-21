#include <assert.h>
#include "contour.h"
#include "image.h"

int main(int argc, char** argv) {
    assert(argc == 2);

    Image I = lire_fichier_image(argv[1]);
    ecrire_image(I);

    Contour c = contour(I);

    liste_points_ecrire(c);
}
