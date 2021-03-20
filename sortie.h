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

#include <stdio.h>
#include "contour.h"
#include "liste_bezier3.h"


typedef enum {
    /* Mode 1 : tracé du contour avec des segments */
    MODE_STROKED,
    /* Mode 2 : tracé du contour avec segments et points */
    MODE_STROKED_POINTS,
    /* Mode 3 : remplissage du contour */
    MODE_FILLED,
} SortieMode;


/**
 * Stocke un fichier de sortie en cours de création. Contient suffisemment d'informations pour ne pas avoir à passer
 * d'arguments redondants dans les appels à `sortie_ecrire_contours`.
 */
typedef struct {
    FILE* out;
    SortieMode mode;
    unsigned int h;
} FichierSortie;


/**
 * Ouvre un fichier de sortie au chemin donné pour une image de taille w x h et écrit l'en-tête.
 * Normalement, une structure crée avec cette fonction doit toujours être libérée/détruite avec `sortie_close`.
 */
FichierSortie sortie_open(char* path, int w, int h, SortieMode mode);


/**
 * Écrit un contour de type segment dans le fichier. Peut être apellé plusieurs fois jusqu'à la fermeture du fichier.
 */
void sortie_ecrire_contour(FichierSortie self, TableauPoints contour);


/**
 * Écrit un contour de type Bezier3 dans le fichier. Peut être apellé plusieurs fois jusqu'à la fermeture du fichier.
 */
void sortie_ecrire_contour_bezier(FichierSortie self, ListeBezier3* contour);


/**
 * Ferme un fichier précédemment ouvert
 */
void sortie_close(FichierSortie self);

#endif
