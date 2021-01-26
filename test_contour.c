#include <assert.h>
#include "contour.h"
#include "image.h"

int main() {
    // Test des opérations génériques sur les listes de points

    ListePointsNoeud n2 = { .value = { 0, 6 }, .next = NULL };
    ListePointsNoeud n1 = { .value = { 0, 4 }, .next = &n2 };
    ListePointsNoeud n0 = { .value = { 0, 2 }, .next = &n1 };
    ListePoints c = { .len = 3, .first = &n0, .last = &n2 };

    ListePointsNoeud n3 = { .value = { 0, 8 }, .next = NULL };
    ListePoints c2 = { .len = 1, .first = &n3, .last = &n3 };

    liste_points_concat(&c, c2);
    assert(c.len == 4);
    assert(c.first == &n0);
    assert(c.last == &n3);

    TableauPoints t = liste_points_to_tableau_points(c);
    assert(t.len == 4);
    assert(t.inner[0].y == n0.value.y);
    assert(t.inner[3].y == n3.value.y);

    // On ne doit pas "supprimer" c et c2, qui allouent leur nœuds sur la pile pour une fois.

    tableau_points_supprimer(&t);

    // Test de l'extraction de contour

    Image I = lire_fichier_image("images/caractere2.pbm");
    Contour image_contour = contour(I);
    supprimer_image(&I);

    liste_points_ecrire(image_contour);
    assert(image_contour.len == 40); // Le contour est-il bon ? On part du principe que oui si le nombre de points est celui attendu

    liste_points_supprimer(&image_contour);

    printf("\e[32mtest_contour passé avec succès !\e[0m\n");
}
