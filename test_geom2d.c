#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "geom2d.h"

int main() {
    Point p1 = set_point(1, 2);
    Point p1_ = set_point(1, ((double)2 / (double)3000) * (double)3000);
    Point p2 = set_point(3, 4);

    assert(0.1 + 0.2 != 0.3); // Témoin pour l'assertion suivante
    assert(egaux_doubles(0.1 + 0.2, 0.3)); // egaux_doubles

    assert(egaux_points(p1, p1_)); // egaux_points
    assert(egaux_points(add_point(p1, p2), set_point(4, 6))); // add_point
    assert(egaux_points(mul_reel_point(p1, 3), set_point(3, 6))); // mul_reel_point
    assert(egaux_doubles(distance(p1, p2), 2 * sqrt(2))); // distance

    Vecteur v1 = set_vecteur(1, 1);
    Vecteur v2 = set_vecteur(2, 2);

    assert(egaux_vecteurs(vect_bipoint(p1, p2), v2)); // vect_bipoint
    assert(egaux_vecteurs(add_vecteur(v1, v2), set_vecteur(3, 3))); // add_vecteur
    assert(egaux_vecteurs(mul_reel_vecteur(v2, 0.5), v1)); // mul_reel_vecteur
    assert(egaux_doubles(produit_scalaire(v1, v2), 4)); // produit_scalaire
    assert(egaux_doubles(norme(v1), sqrt(2))); // norme

    // Distance point-segment

    Point a = set_point(0, 0), b = set_point(2, 0);

    assert(egaux_doubles(distance_point_segment(b, b, a), 2)); // segment de taille nulle

    assert(egaux_doubles(distance_point_segment(a, b, set_point(1, 0)), 0)); // point sur le segment
    assert(egaux_doubles(distance_point_segment(a, b, set_point(2, 0)), 0)); // point sur le segment
    assert(egaux_doubles(distance_point_segment(a, b, set_point(-1, 0)), 1)); // point avant le segment, sur la gauche de a
    assert(egaux_doubles(distance_point_segment(a, b, set_point(3, 1)), sqrt(2))); // point après le segment, au dessus à droite de b
    assert(egaux_doubles(distance_point_segment(a, b, set_point(0, 0.5)), 0.5)); // point au "dessus" de a
    assert(egaux_doubles(distance_point_segment(a, b, set_point(0.5, 1)), 1)); // point au "dessus" du milieu du segment

    printf("\e[32mtest_geom2d passé avec succès !\e[0m\n");
}
