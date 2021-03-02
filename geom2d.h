#ifndef _GEOM2D_H_
#define _GEOM2D_H_

#include <stdbool.h>

typedef struct Vecteur_ {
    double x, y;
} Vecteur;


typedef struct Point_ {
    double x, y;
} Point;


/*
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

#endif
