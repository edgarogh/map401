#include <assert.h>
#include <stdlib.h>
#include "geom2d.h"
#include "simplification.h"


ListeBezier3 simplification_douglas_peucker_bezier2(TableauPoints c, unsigned int i1, unsigned int i2, double seuil) {
    assert(i1 < i2);

    unsigned int n = i2 - i1;
    unsigned int np1 = n + 1;
    Bezier2 bezier = approx_bezier2(&c.inner[i1], np1);

    double distance_max = 0;
    int index_distance_max = i1;
    for (int i = i1 + 1; i <= i2; i++) {
        int j = i - i1;
        double distance = distance_point_bezier2(&bezier, c.inner[i], (float) j / (float) n);
        if (distance_max < distance) {
            distance_max = distance;
            index_distance_max = i;
        }
    }

    ListeBezier3 L;
    if (distance_max <= seuil) {
        L = liste_bezier3_new();
        liste_bezier3_push(&L, bezier2_to_bezier3(&bezier));
    } else {
        L = simplification_douglas_peucker_bezier2(c, i1, index_distance_max, seuil);
        ListeBezier3 L2 = simplification_douglas_peucker_bezier2(c, index_distance_max, i2, seuil);

        liste_bezier3_concat(&L, L2);
    }

    return L;
}


ListePoint simplification_douglas_peucker(TableauPoints c, unsigned int i1, unsigned int i2, double seuil) {
    double distance_max = 0;
    int index_distance_max = i1;
    for (int i = i1 + 1; i <= i2; i++) {
        double distance = distance_point_segment(c.inner[i1], c.inner[i2], c.inner[i]);
        if (distance_max < distance) {
            distance_max = distance;
            index_distance_max = i;
        }
    }

    ListePoint L;
    if (distance_max <= seuil) {
        L = liste_point_new();
        liste_point_push(&L, c.inner[i1]);
        liste_point_push(&L, c.inner[i2]);
    } else {
        L = simplification_douglas_peucker(c, i1, index_distance_max, seuil);
        ListePoint L2 = simplification_douglas_peucker(c, index_distance_max, i2, seuil);

        // Le dernier élément de L est le même que le premier élément de L2, on enlève donc le 1er élément de L2
        if (L2.first) {
            ListePointNoeud* first = L2.first;
            ListePointNoeud* second = first->next;
            L2.first = second;
            free(first);
            L2.len--;
        }

        liste_point_concat(&L, L2);
    }

    return L;
}
