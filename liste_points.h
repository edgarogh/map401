#include "geom2d.h"


/**
 * Représente un nœud de la liste chaînée `ListePoints` définie ci-dessous
 */
typedef struct ListePointsNoeud_ {
    Point value;
    struct ListePointsNoeud_* next;
} ListePointsNoeud;


/**
 * Liste chaînée de points
 *
 * Le champ `last` et le champ `len` sont redondants car ces deux informations peuvent être obtenues qu'à partir de la
 * tête, mais leur présence permet de faire certaines opérations en O(1) plutôt que O(n).
 */
typedef struct {
    /**
     * Longueur du contour / Nombre d'éléments
     */
    unsigned int len;

    ListePointsNoeud* first;
    ListePointsNoeud* last;
} ListePoints;


ListePoints liste_points_new();


void liste_points_supprimer(ListePoints* self);


/**
 * Concatène `other` en queue de `self`, modifiant `self`
 */
void liste_points_concat(ListePoints* self, ListePoints other);


void liste_points_push(ListePoints* self, Point value);


void liste_points_ecrire(ListePoints l);


typedef struct {
    unsigned int len;
    Point* inner;
} TableauPoints;


TableauPoints liste_points_to_tableau_points(ListePoints self);


void tableau_points_supprimer(TableauPoints* self);
