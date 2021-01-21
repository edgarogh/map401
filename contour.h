#include "liste_points.h"
#include "image.h"


typedef enum { Nord = 0, Est = 1, Sud = 2, Ouest = 3 } Orientation;


typedef ListePoints Contour;


/**
 * Renvoie une liste de points représentant un polygone correspondant aux pixels noirs de l'image. La fonction ne gère
 * pas la situation où plusieurs "zones" indépendantes de pixels noirs existeraient.
 *
 * @param I
 * @return
 */
Contour contour(Image I);
