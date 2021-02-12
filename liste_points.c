#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "liste_points.h"


ListePoints liste_points_new() {
    return (ListePoints) {
            .len = 0,
            .first = NULL,
            .last = NULL,
    };
}


void liste_points_supprimer(ListePoints* self) {
    ListePointsNoeud* noeud = self->first;

    while (noeud) {
        ListePointsNoeud* tmp = noeud->next;
        free(noeud);
        noeud = tmp;
    }

    self->first = self->last = NULL;
    self->len = 0;
}


void liste_points_concat(ListePoints* self, ListePoints other) {
    self->len += other.len;
    self->last->next = other.first;
    self->last = other.last;
}


void liste_points_push(ListePoints* self, Point value) {
    ListePointsNoeud* new_node = malloc(sizeof(ListePointsNoeud));
    new_node->next = NULL;
    new_node->value = value;
    self->len = self->len + 1;

    if (self->first) {
        self->last->next = new_node;
        self->last = new_node;
    } else {
        self->first = new_node;
        self->last = new_node;
    }
}


void liste_points_ecrire(ListePoints l) {
    ListePointsNoeud* n = l.first;

    printf("contour (%d segments) = [\n", l.len);
    while (n) {
        printf("  (%f, %f)\n", n->value.x, n->value.y);
        n = n->next;
    }
    printf("]\n");
}


TableauPoints liste_points_to_tableau_points(ListePoints self) {
    TableauPoints t = { .len = self.len, .inner = malloc(self.len * sizeof(Point)) };

    ListePointsNoeud* noeud = self.first;

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
