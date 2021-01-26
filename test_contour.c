#include <assert.h>
#include "contour.h"
#include "image.h"

int main() {
    Image I = lire_fichier_image("images/caractere2.pbm");
    Contour c = contour(I);
    supprimer_image(&I);

    liste_points_ecrire(c);
    assert(c.len == 40); // Le contour est-il bon ? On part du principe que oui si le nombre de points est celui attendu

    liste_points_supprimer(&c);

    printf("\e[32mtest_contour passé avec succès !\e[0m\n");
}
