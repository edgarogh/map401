#ifndef _SIMPLIFICATION_H_
#define _SIMPLIFICATION_H_

#include "liste_bezier2.h"
#include "tableau_points.h"


/**
 * Renvoie une courbe de bézier approximant le polygone définit par les `len` points à partir de `start`
 */
Bezier2 approx_bezier2(Point *start, unsigned int len);


ListeBezier2 simplification_douglas_peucker_bezier2(TableauPoints c, int i1, int i2, double seuil);


ListePoint simplification_douglas_peucker(TableauPoints c, int i1, int i2, double seuil);

#endif
