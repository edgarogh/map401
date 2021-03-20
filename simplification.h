#ifndef _SIMPLIFICATION_H_
#define _SIMPLIFICATION_H_

#include "liste_bezier3.h"
#include "tableau_points.h"


/**
 * Renvoie une courbe de bézier approximant le polygone définit par les `len` points à partir de `start`
 */
Bezier2 approx_bezier2(Point *start, unsigned int len);


/**
 * Simplifie une séquence de points avec l'algo. de Douglas-Peucker, entre un intervalle i1..i2, pour un seuil donné, en
 * utilisant des courbes de Bézier de degré 2 pour remplacer les sous-séquences de points à simplifier.
 */
ListeBezier3 simplification_douglas_peucker_bezier2(TableauPoints c, unsigned int i1, unsigned int i2, double seuil);

/**
 * Simplifie une séquence de points avec l'algo. de Douglas-Peucker, entre un intervalle i1..i2, pour un seuil donné, en
 * utilisant des segments pour remplacer les sous-séquences de points à simplifier.
 */
ListePoint simplification_douglas_peucker(TableauPoints c, unsigned int i1, unsigned int i2, double seuil);

#endif
