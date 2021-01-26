#include <assert.h>
#include "contour.h"
#include "image.h"

int main() {
    Image I = lire_fichier_image("images/ligne.pbm");
    ecrire_image(I);
    contour(I);

    I = lire_fichier_image("images/test_contour_carreau.pbm");
    ecrire_image(I);
    contour(I);
}
