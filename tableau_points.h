#ifndef _TABLEAU_POINTS_H_
#define _TABLEAU_POINTS_H_

#include <stdio.h>
#include "liste_point.h"
#include "geom2d.h"


void liste_point_ecrire(ListePoint l);


typedef struct {
    unsigned int len;
    Point* inner;
} TableauPoints;


TableauPoints liste_point_to_tableau_points(ListePoint self);


void tableau_points_supprimer(TableauPoints* self);


/**
 * Enregistre un tableau de points dans le format spécifié dans `Tache3.pdf`. Ne ferme pas le fichier, dont la gestion
 * est laissée à l'apelleur.
 */
void tableau_points_enregistrer(TableauPoints* self, FILE* f);

#endif
