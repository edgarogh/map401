#ifndef _GEOM2D_H_
#define _GEOM2D_H_

#include <stdbool.h>

typedef struct Vecteur_ {
    double x, y;
} Vecteur;


typedef struct Point_ {
    double x, y;
} Point;


typedef struct Bezier2_ {
    Point c0, c1, c2;
} Bezier2;


typedef struct Bezier3_ {
    Point c0, c1, c2, c3;
} Bezier3;


/**
 * Renvoie `true` si `d1` et `d2` peuvent être considérés comme égaux en tenant compte de la précision approximative des
 * nombres flottants.
 */
bool egaux_doubles(double d1, double d2);


// Points


Point set_point(double x, double y);


Point add_point(Point p1, Point p2);


Point mul_reel_point(Point p, double a);


double distance(Point p1, Point p2);


bool egaux_points(Point p1, Point p2);


// Vecteurs


Vecteur set_vecteur(double x, double y);


Vecteur add_vecteur(Vecteur v1, Vecteur v2);


Vecteur mul_reel_vecteur(Vecteur v, double a);


Vecteur vect_bipoint(Point a, Point b);


double produit_scalaire(Vecteur v1, Vecteur v2);


double norme(Vecteur v);


bool egaux_vecteurs(Vecteur v1, Vecteur v2);


// Distance point-segment


double distance_point_segment(Point segment_a, Point segment_b, Point p);


// Courbes de bezier


/**
 * Calcule C(t) où C est une courbe de Bézier de degré 2 passée en premier paramètre
 */
Point bezier2_C(Bezier2* self, double t);


/**
 * Approxime une distance entre le point `self`(`t`) et le point `p`, où `self` est une courbe de Bézier de degré 2.
 */
double distance_point_bezier2(Bezier2* self, Point p, double t);


/**
 * Calcule C(t) où C est une courbe de Bézier de degré 3 passée en premier paramètre
 */
Point bezier3_C(Bezier3* self, double t);


/**
 * Approxime une distance entre le point `self`(`t`) et le point `p`, où `self` est une courbe de Bézier de degré 3.
 */
double distance_point_bezier3(Bezier3* self, Point p, double t);


/**
 * Incrémente le degré d'une courbe de Bezier de degré 2
 */
Bezier3 bezier2_to_bezier3(Bezier2* self);


/**
 * Approxime la séquence de points start[0]..start[len-1] par une courbe de Bézier de degré 2 dont les points de
 * contrôle extrêmes sont le début et la fin de la séquence.
 */
Bezier2 approx_bezier2(Point* start, unsigned int len);


/**
 * Approxime la séquence de points start[0]..start[len-1] par une courbe de Bézier de degré 3 dont les points de
 * contrôle extrêmes sont le début et la fin de la séquence.
 */
Bezier3 approx_bezier3(Point* start, unsigned int len);

#endif
