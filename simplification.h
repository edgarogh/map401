#ifndef _SIMPLIFICATION_H_
#define _SIMPLIFICATION_H_

#include "liste_points.h"

/**
 * Renvoie une courbe de bézier approximant le polygone définit par les `len` points à partir de `start`
 */
Bezier2 approx_bezier2(Point *start, unsigned int len);



ListePoints simplification_douglas_peucker(TableauPoints c, int i1, int i2, double seuil);

#endif
