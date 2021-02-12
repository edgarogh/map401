#ifndef _CONTOUR_H_
#define _CONTOUR_H_

#include "liste_points.h"
#include "image.h"


typedef enum { Nord = 0, Est = 1, Sud = 2, Ouest = 3 } Orientation;


typedef ListePoints Contour;


/**
 * Une image destinée à être utilisée comme un masque. Cet alias existe simplement pour clarifier les paramètres et les
 * types de retour de certaines fonctions.
 */
typedef Image Mask;


/**
 * Créé un masque initial pour l'extraction des contours de I
 */
Mask contour_init_mask(Image I);


/**
 * Renvoie une liste de points représentant un polygone correspondant aux pixels noirs de l'image. Le masque permet à la
 * fonction d'opérer sur des iamges posssédant plusieurs contours. En passant une référence au même masque (initialisé
 * avec `contour_init_mask`, la fonction renverra successivement, appel par appel, tous les contours qu'elle trouve
 * jusqu'à ce qu'il n'y en ait plus et qu'elle renvoie un contour vide.
 */
Contour contour(Image I, Mask mut_mask);

#endif
