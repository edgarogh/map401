#include <stdlib.h>
#include "geom2d.h"
#include "simplification.h"


typedef struct {
    Point a, b;
} Segment;


ListePoints simplification_douglas_peucker_(TableauPoints T, int j1, int j2, double d){
    double dmax = 0;
    double dj;
    int k = j1;
    ListePoints Liste = liste_points_new();
    ListePoints *L = &Liste;

    Segment Pj1j2 = { T.inner[j1], T.inner[j2] };

    for (int j=j1+1; j<=j2; j++){
        dj = distance_point_segment(Pj1j2.a, Pj1j2.b, T.inner[j]);
        if (dmax < dj){
            dmax = dj;
            k = j;
        }
    }

    if (dmax <= d){
        liste_points_push(L, T.inner[j1]);
        liste_points_push(L, T.inner[j2]);
    }
    else{
        Liste = simplification_douglas_peucker(T, j1, k, d);
        liste_points_concat(L, simplification_douglas_peucker(T, k, j2, d));
    }

    return Liste;
}
