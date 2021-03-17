#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "liste_ttttt.h"


ListeTTTTT liste_ttttt_new() {
    return (ListeTTTTT) {
            .len = 0,
            .first = NULL,
            .last = NULL,
    };
}


void liste_ttttt_supprimer(ListeTTTTT* self) {
    ListeTTTTTNoeud* noeud = self->first;

    while (noeud) {
        ListeTTTTTNoeud* tmp = noeud->next;
        free(noeud);
        noeud = tmp;
    }

    self->first = self->last = NULL;
    self->len = 0;
}


void liste_ttttt_concat(ListeTTTTT* self, ListeTTTTT other) {
    self->len += other.len;
    self->last->next = other.first;
    self->last = other.last;
}


void liste_ttttt_push(ListeTTTTT* self, TTTTT value) {
    ListeTTTTTNoeud* new_node = malloc(sizeof(ListeTTTTTNoeud));
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
