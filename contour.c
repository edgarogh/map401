#include <stdio.h>
#include "contour.h"
#include "types_macros.h"


Point contour_trouver_pixel_depart(Image I) {
    FOR_EACH_PIXEL(I, x, y) {
        if (get_pixel_image(I, x, y) == NOIR && get_pixel_image(I, x, y - 1) == BLANC) {
            return set_point(x, y);
        }
    }

    ERREUR_FATALE("Impossible de trouver un pixel de départ, l'image est-elle vide ?");
}


void contour_avancer(Point* current_position, Orientation direction) {
    switch (direction) {
        case Nord:
            current_position->y--;
            break;
        case Est:
            current_position->x++;
            break;
        case Sud:
            current_position->y++;
            break;
        case Ouest:
            current_position->x--;
            break;
    }
}


Orientation contour_gauche(Orientation o) {
    // Les orientations sont en réalité des entiers dans [0; 4[
    return (o + 3) % 4;
}


Orientation contour_droit(Orientation o) {
    // Les orientations sont en réalité des entiers dans [0; 4[
    return (o + 1) % 4;
}


Orientation contour_nouvelle_orientation(Image I, Point current_position, Orientation current_orientation) {
    Pixel g;
    Pixel d;

    switch (current_orientation) {
        case Nord:
            g = get_pixel_image(I, current_position.x, current_position.y);
            d = get_pixel_image(I, current_position.x + 1, current_position.y);
            break;
        case Est:
            g = get_pixel_image(I, current_position.x + 1, current_position.y);
            d = get_pixel_image(I, current_position.x + 1, current_position.y + 1);
            break;
        case Sud:
            g = get_pixel_image(I, current_position.x + 1, current_position.y + 1);
            d = get_pixel_image(I, current_position.x, current_position.y + 1);
            break;
        case Ouest:
            g = get_pixel_image(I, current_position.x, current_position.y + 1);
            d = get_pixel_image(I, current_position.x, current_position.y);
            break;
    }

    if (g == NOIR) return contour_gauche(current_orientation);
    if (d == BLANC) return contour_droit(current_orientation);
    return current_orientation;
}


void contour(Image I) {
    Point start = contour_trouver_pixel_depart(I);
    // Coin haut-gauche du pixel
    start = add_point(start, set_point(-1, -1));

    Point current_position = start;
    Orientation current_orientation = Est;

    while (true) {
        printf("point du contour: %f %f\n", current_position.x, current_position.y);

        contour_avancer(&current_position, current_orientation);
        current_orientation = contour_nouvelle_orientation(I, current_position, current_orientation);

        if (egaux_points(current_position, start) && (current_orientation == Est)) {
            break;
        }
    }

    printf("point du contour: %f %f\n", current_position.x, current_position.y);
}
