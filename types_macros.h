/****************************************************************************** 
  declaration de differents types et macros
******************************************************************************/
 
#ifndef _TYPES_MACROS_H_
#define _TYPES_MACROS_H_

#include <stdio.h>
#include <stdlib.h>

/* type booleen */
typedef char bool;
#define false 0
#define true  1

/* type entier positif */
typedef unsigned int UINT;

/* macro affichant le message _m_ et s'arretant */
#define ERREUR_FATALE(_m_) \
{ \
fprintf(stderr, "%s\n", _m_); \
exit(1); \
}

/* macro facilitant l'itération sur des images */
#define FOR_EACH_PIXEL(IMG, VAR_X, VAR_Y) \
for (UINT VAR_Y = 1; VAR_Y <= IMG.H; VAR_Y++) \
for (UINT VAR_X = 1; VAR_X <= IMG.L; VAR_X++)

#endif
