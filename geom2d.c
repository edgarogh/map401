#include <math.h>
#include "geom2d.h"

#define DOUBLE_EPSILON 0.0000000001


bool egaux_doubles(double d1, double d2) {
    return fabs(d1 - d2) < DOUBLE_EPSILON;
}


// Points


Point set_point(double x, double y) {
    return (Point) { x, y };
}


Point add_point(Point p1, Point p2) {
    return set_point(p1.x + p2.x, p1.y + p2.y);
}


Point mul_reel_point(Point p, double a) {
    return set_point(a * p.x, a * p.y);
}


double distance(Point p1, Point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}


bool egaux_points(Point p1, Point p2) {
    return egaux_doubles(p1.x, p2.x) && egaux_doubles(p1.y, p2.y);
}


// Vecteurs


Vecteur set_vecteur(double x, double y) {
    return (Vecteur) { x, y };
}


Vecteur add_vecteur(Vecteur v1, Vecteur v2) {
    return set_vecteur(v1.x + v2.x, v1.y + v2.y);
}


Vecteur mul_reel_vecteur(Vecteur v, double a) {
    return set_vecteur(a * v.x, a * v.y);
}


Vecteur vect_bipoint(Point a, Point b) {
    return set_vecteur(b.x - a.x, b.y - a.y);
}


double produit_scalaire(Vecteur v1, Vecteur v2) {
    return (v1.x * v2.x) + (v1.y * v2.y);
}


double norme(Vecteur v) {
    return sqrt(pow(v.x, 2) + pow(v.y, 2));
}


bool egaux_vecteurs(Vecteur v1, Vecteur v2) {
    return egaux_doubles(v1.x, v2.x) && egaux_doubles(v1.y, v2.y);
}


// Distance point-segment


double distance_point_segment(Point a, Point b, Point p) {
    if (egaux_points(a, b)) return distance(a, p);

    Vecteur ab = vect_bipoint(a, b);
    double lambda = produit_scalaire(vect_bipoint(a, p), ab) / produit_scalaire(ab, ab);

    if (lambda < 0) {
        return distance(a, p);
    } else if (lambda > 1) {
        return distance(b, p);
    } else {
        Point q = add_point(a, mul_reel_point(add_point(b, mul_reel_point(a, -1)), lambda));
        return distance(q, p);
    }
}
