#ifndef _SIMPLIFICATION_H_
#define _SIMPLIFICATION_H_

#include "liste_points.h"


ListePoints simplification_douglas_peucker(TableauPoints c, int i1, int i2, double seuil);

#endif
