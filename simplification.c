#include <stdlib.h>
#include "geom2d.h"
#include "simplification.h"


ListePoints simplification_douglas_peucker(TableauPoints c, int i1, int i2, double seuil) {
    double distance_max = 0;
    int index_distance_max = i1;
    for (int i = i1 + 1; i <= i2; i++) {
        double distance = distance_point_segment(c.inner[i1], c.inner[i2], c.inner[i]);
        if (distance_max < distance) {
            distance_max = distance;
            index_distance_max = i;
        }
    }

    ListePoints L;
    if (distance_max <= seuil) {
        L = liste_points_new();
        liste_points_push(&L, c.inner[i1]);
        liste_points_push(&L, c.inner[i2]);
    } else {
        L = simplification_douglas_peucker(c, i1, index_distance_max, seuil);
        ListePoints L2 = simplification_douglas_peucker(c, index_distance_max, i2, seuil);

        // Le dernier élément de L est le même que le premier élément de L2, on enlève donc le 1er élément de L2
        if (L2.first) {
            ListePointsNoeud* first = L2.first;
            ListePointsNoeud* second = first->next;
            L2.first = second;
            free(first);
            L2.len--;
        }

        liste_points_concat(&L, L2);
    }

    return L;
}
