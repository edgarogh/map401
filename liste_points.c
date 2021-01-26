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

    printf("contour = [\n");
    while (n) {
        printf("  (%f, %f)\n", n->value.x, n->value.y);
        n = n->next;
    }
    printf("]\n");
}
