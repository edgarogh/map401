#include <assert.h>
#include <stdlib.h>
#include "geom2d.h"
#include "simplification.h"


Bezier2 approx_bezier2(Point *start, unsigned int len) {
    assert(len > 1);

    Bezier2 curve = {
            .c0 = start[0],
            .c2 = start[1],
    };

    if (len == 2) {
        curve.c1 = mul_reel_point(add_point(start[0], start[1]), .5);
    } else {
        int n = (int)len - 1;

        Point sum = set_point(0, 0);

        for (int i = 1; i < (n-1); i++) {
            sum = add_point(sum, start[i]);
        }

        float alpha = 3*n / (n*n - 1);
        float beta = (1 - 2*n) / (2 * (n + 1));

        curve.c1 = add_point(mul_reel_point(sum, alpha), mul_reel_point(add_point(start[0], start[len-1]), beta));
    }

    return curve;
}


ListeBezier2 simplification_douglas_peucker_bezier2(TableauPoints c, int i1, int i2, double seuil) {
    // TODO
    exit(1);
}


ListePoint simplification_douglas_peucker(TableauPoints c, int i1, int i2, double seuil) {
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
