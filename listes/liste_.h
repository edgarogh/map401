#ifndef _LISTE_TTTTT_H_
#define _LISTE_TTTTT_H_

#include <stdio.h>
#INCLUDE_TEMPLATE


/**
 * Représente un nœud de la liste chaînée définie ci-dessous
 */
typedef struct ListeTTTTTNoeud_ {
    TTTTT value;
    struct ListeTTTTTNoeud_* next;
} ListeTTTTTNoeud;


/**
 * Liste chaînée
 *
 * Le champ `last` et le champ `len` sont redondants car ces deux informations peuvent être obtenues qu'à partir de la
 * tête, mais leur présence permet de faire certaines opérations en O(1) plutôt que O(n).
 */
typedef struct {
    /**
     * Longueur du contour / Nombre d'éléments
     */
    unsigned int len;

    ListeTTTTTNoeud* first;
    ListeTTTTTNoeud* last;
} ListeTTTTT;


ListeTTTTT liste_ttttt_new();


void liste_ttttt_supprimer(ListeTTTTT* self);


/**
 * Concatène `other` en queue de `self`, modifiant `self`
 */
void liste_ttttt_concat(ListeTTTTT* self, ListeTTTTT other);


void liste_ttttt_push(ListeTTTTT* self, TTTTT value);

#endif
