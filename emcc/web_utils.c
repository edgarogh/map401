#include <unistd.h>
#include <stdio.h>
#include "../image.h"
#include "../contour.h"

Image* web_lire_fichier_image(int id) {
    char name[20];
    sprintf(name, "/%d.pbm", id);
    Image* ptr = malloc(sizeof(Image));
    *ptr = lire_fichier_image(name);
    return ptr;
}

int web_image_width(Image* i) {
    return i->L;
}

int web_image_height(Image* i) {
    return i->H;
}

void web_image_free(Image* i) {
    supprimer_image(i);
    free(i);
}

TableauPoints* web_contour(Image* i) {
    TableauPoints* ptr = malloc(sizeof(TableauPoints));
    *ptr = liste_points_to_tableau_points(contour(*i));
    return ptr;
}

int web_contour_len(TableauPoints* t) {
    return t->len;
}

Point* web_contour_points(TableauPoints* t) {
    return t->inner;
}
