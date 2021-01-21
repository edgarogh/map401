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


void liste_points_push(ListePoints* self, Point value);


void liste_points_ecrire(ListePoints l);
