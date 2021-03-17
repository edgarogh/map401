#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "tableau_points.h"


void liste_point_ecrire(ListePoint l) {
    ListePointNoeud* n = l.first;

    printf("contour (%d segments) = [\n", l.len);
    while (n) {
        printf("  (%f, %f)\n", n->value.x, n->value.y);
        n = n->next;
    }
    printf("]\n");
}


TableauPoints liste_point_to_tableau_points(ListePoint self) {
    TableauPoints t = { .len = self.len, .inner = malloc(self.len * sizeof(Point)) };

    ListePointNoeud* noeud = self.first;

    for (int i = 0; i < t.len; i++) {
        // Impossible que ça rate en temps normal, ça peut se produire si `self.len` est désynchronisé avec la longueur
        // effective de la liste chaînée.
        assert(noeud != NULL);

        t.inner[i] = noeud->value;

        noeud = noeud->next;
    }

    return t;
}


void tableau_points_supprimer(TableauPoints* self) {
    free(self->inner);
}


void tableau_points_enregistrer(TableauPoints* self, FILE* f) {
    fprintf(f, "\n%d\n", self->len);
    for (int i = 0; i < self->len; i++) {
        Point p = self->inner[i];
        fprintf(f, " %f %f\n", p.x, p.y);
    }
}
