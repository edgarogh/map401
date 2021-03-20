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

    printf("\e[32mtest_geom2d:base passé avec succès !\e[0m\n");

    // Distance point-segment

    Point a = set_point(0, 0), b = set_point(2, 0);

    assert(egaux_doubles(distance_point_segment(b, b, a), 2)); // segment de taille nulle

    assert(egaux_doubles(distance_point_segment(a, b, set_point(1, 0)), 0)); // point sur le segment
    assert(egaux_doubles(distance_point_segment(a, b, set_point(2, 0)), 0)); // point sur le segment
    assert(egaux_doubles(distance_point_segment(a, b, set_point(-1, 0)), 1)); // point avant le segment, sur la gauche de a
    assert(egaux_doubles(distance_point_segment(a, b, set_point(3, 1)), sqrt(2))); // point après le segment, au dessus à droite de b
    assert(egaux_doubles(distance_point_segment(a, b, set_point(0, 0.5)), 0.5)); // point au "dessus" de a
    assert(egaux_doubles(distance_point_segment(a, b, set_point(0.5, 1)), 1)); // point au "dessus" du milieu du segment

    printf("\e[32mtest_geom2d:point_segment passé avec succès !\e[0m\n");

    // Bezier

    Bezier2 bezier2 = { .c0 = set_point(0, 0), .c1 = set_point(0, 2), .c2 = set_point(2, 2) };
    assert(egaux_points(bezier2_C(&bezier2, 0), set_point(0, 0)));
    assert(egaux_points(bezier2_C(&bezier2, 1), set_point(2, 2)));
    assert(egaux_points(bezier2_C(&bezier2, .5), set_point(0.5, 1.5)));

    Bezier3 bezier3 = { .c0 = { 0., 0. }, .c1 = { 1., -1. }, .c2 = { 2., 3. }, .c3 = { 3, 2 } };
    assert(egaux_points(bezier3_C(&bezier3, 0), set_point(0, 0)));
    assert(egaux_points(bezier3_C(&bezier3, 1), set_point(3, 2)));
    assert(egaux_points(bezier3_C(&bezier3, .5), set_point(1.5, 1)));
    assert(egaux_points(bezier3_C(&bezier3, .2), set_point(0.6, -0.08)));
    assert(egaux_points(bezier3_C(&bezier3, .7), set_point(2.1, 1.82)));

    Bezier3 bezier2to3 = bezier2_to_bezier3(&bezier2);
    assert(egaux_points(bezier2to3.c0, bezier2.c0));
    assert(egaux_points(bezier2to3.c3, bezier2.c2));
    assert(egaux_points(bezier2to3.c1, set_point(0, 4./3.)));
    assert(egaux_points(bezier2to3.c2, set_point(2./3., 2)));

    // approx_bezier2

    // n=1
    bezier2 = approx_bezier2((Point[]) { set_point(0., 2.), set_point(2., 0.) }, 2);
    assert(egaux_points(bezier2.c0, set_point(0., 2.)));
    assert(egaux_points(bezier2.c1, set_point(1., 1.)));
    assert(egaux_points(bezier2.c2, set_point(2., 0.)));

    // n≥2
    Bezier2 bezier2base = { .c0 = { 4.5, 6. }, .c1 = { -7., 0.2 }, .c2 = { 3., -3. } };
    Point points[] = {
            bezier2_C(&bezier2base, 0),
            bezier2_C(&bezier2base, 0.25),
            bezier2_C(&bezier2base, 0.5),
            bezier2_C(&bezier2base, 0.75),
            bezier2_C(&bezier2base, 1),
    };
    bezier2 = approx_bezier2(points, 5);
    printf("(%f;%f)\n", bezier2.c1.x, bezier2.c1.y);
    assert(egaux_points(bezier2base.c0, bezier2.c0));
    assert(egaux_points(bezier2base.c1, bezier2.c1));
    assert(egaux_points(bezier2base.c2, bezier2.c2));

    // n=9 (exemple du cours)
    Point points2[] = {
            { 0., 0. },
            { 1., 0. },
            { 1., 1. },
            { 1., 2. },
            { 2., 2. },
            { 3., 2. },
            { 3., 3. },
            { 4., 3. },
            { 5., 3. },
    };
    bezier2 = approx_bezier2(points2, 9);
    // Je fais tout ça pour ne comparer que les 6 premiers chiffres significatifs
    int xe5 = (1.547619 - bezier2.c1.x) * 100000;
    int ye5 = (2.452381 - bezier2.c1.y) * 100000;
    assert(xe5 == 0 && ye5 == 0);

    printf("\e[32mtest_geom2d:bezier passé avec succès !\e[0m\n");

    printf("\e[32mtest_geom2d:* passé avec succès !\e[0m\n");
}
