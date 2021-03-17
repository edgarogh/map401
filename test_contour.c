#include <assert.h>
#include "contour.h"
#include "image.h"

int main() {
    // === Test des opérations génériques sur les listes de points ===

    ListePointNoeud n2 = { .value = { 0, 6 }, .next = NULL };
    ListePointNoeud n1 = { .value = { 0, 4 }, .next = &n2 };
    ListePointNoeud n0 = { .value = { 0, 2 }, .next = &n1 };
    ListePoint c = { .len = 3, .first = &n0, .last = &n2 };

    ListePointNoeud n3 = { .value = { 0, 8 }, .next = NULL };
    ListePoint c2 = { .len = 1, .first = &n3, .last = &n3 };

    liste_point_concat(&c, c2);
    assert(c.len == 4);
    assert(c.first == &n0);
    assert(c.last == &n3);

    TableauPoints t = liste_point_to_tableau_points(c);
    assert(t.len == 4);
    assert(t.inner[0].y == n0.value.y);
    assert(t.inner[3].y == n3.value.y);

    // On ne doit pas "supprimer" c et c2, qui allouent leur nœuds sur la pile pour une fois.

    tableau_points_supprimer(&t);

    // === Test de l'extraction de contour sur un contour unique ===

    Image i = lire_fichier_image("images/caractere2.pbm");
    Mask m = contour_init_mask(i);
    Contour image_contour = contour(i, m);
    supprimer_image(&m);
    supprimer_image(&i);

    liste_point_ecrire(image_contour);
    assert(image_contour.len == 41); // Le contour est-il bon ? On part du principe que oui si le nombre de points est celui attendu

    liste_point_supprimer(&image_contour);

    // === Test de l'extraction de contour sur des contours multiples ===

    // image2_poly.pbm

    i = lire_fichier_image("images/image2_poly.pbm");
    m = contour_init_mask(i);
    Contour image_contour1 = contour(i, m);
    Contour image_contour2 = contour(i, m);
    Contour image_contour3 = contour(i, m);
    Contour image_contour_fin = contour(i, m);
    supprimer_image(&m);
    supprimer_image(&i);

    assert(image_contour_fin.len == 0); // Seulement 3 contours, le 4ème est "vide" = inexistant
    assert(image_contour1.len == 23);
    assert(image_contour2.len == 11);
    assert(image_contour3.len == 25);

    liste_point_supprimer(&image_contour1);
    liste_point_supprimer(&image_contour2);
    liste_point_supprimer(&image_contour3);
    liste_point_supprimer(&image_contour_fin);

    // cible

    i = lire_fichier_image("images/cible.pbm");
    m = contour_init_mask(i);
    image_contour1 = contour(i, m);
    image_contour2 = contour(i, m);
    image_contour3 = contour(i, m);
    Contour image_contour4 = contour(i, m);
    image_contour_fin = contour(i, m);
    supprimer_image(&m);
    supprimer_image(&i);

    assert(image_contour_fin.len == 0);
    assert(image_contour1.len-1 + image_contour2.len-1 + image_contour3.len-1 + image_contour4.len-1 == 64);

    liste_point_supprimer(&image_contour1);
    liste_point_supprimer(&image_contour2);
    liste_point_supprimer(&image_contour3);
    liste_point_supprimer(&image_contour4);
    liste_point_supprimer(&image_contour_fin);

    // tetris

    i = lire_fichier_image("images/tetris.pbm");
    m = contour_init_mask(i);
    image_contour1 = contour(i, m);
    image_contour2 = contour(i, m);
    image_contour_fin = contour(i, m);
    supprimer_image(&m);
    supprimer_image(&i);

    assert(image_contour_fin.len == 0);
    assert(image_contour1.len-1 + image_contour2.len-1 == 18);

    liste_point_supprimer(&image_contour1);
    liste_point_supprimer(&image_contour2);
    liste_point_supprimer(&image_contour_fin);

    // === (fin) ===

    printf("\e[32mtest_contour passé avec succès !\e[0m\n");
}
