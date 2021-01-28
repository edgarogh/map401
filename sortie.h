/**
 * Le projet n'est censé utiliser que du PostScript en sortie en pratique, mais ce fichier est "neutre" vis-à-vis du
 * format de sortie, si bien qu'on pourrait supporter du SVG (par exemple) en changeant uniquement le fichier C utilisé
 * pour générer sortie.o dans le Makefile.
 *
 * Dit autrement, ce fichier ("sortie.h") ne fais aucun a-priori sur le format de fichier vers lequel on écrit, seule
 * la/les implémentation(s) le détermineront.
 */

#ifndef _SORTIE_H_
#define _SORTIE_H_

#include "contour.h"


typedef enum {
    /* Mode 1 : tracé du contour avec des segments */
    MODE_STROKED,
    /* Mode 2 : tracé du contour avec segments et points */
    MODE_STROKED_POINTS,
    /* Mode 3 : remplissage du contour */
    MODE_FILLED,
} SortieMode;


/**
 * Ouvre un fichier de sortie au chemin donné pour une image de taille w x h, écrit dedans et le ferme.
 */
void sortie_ecrire_contour(char* path, int w, int h, TableauPoints contour, SortieMode mode);

#endif
