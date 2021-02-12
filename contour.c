#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "contour.h"
#include "types_macros.h"


bool contour_trouver_pixel_noir(Image I, Point* mut_return) {
    FOR_EACH_PIXEL(I, x, y) {
        if (get_pixel_image(I, x, y) == NOIR && get_pixel_image(I, x, y - 1) == BLANC) {
            *mut_return = set_point(x, y);
            return true;
        }
    }

    return false;
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


Mask contour_init_mask(Image I) {
    Image mask = creer_image(I.L, I.H);

    FOR_EACH_PIXEL(mask, x, y) {
        Pixel p;
        if ((get_pixel_image(I, x, y) == NOIR) && (get_pixel_image(I, x, y - 1) == BLANC)) {
            p = NOIR;
        } else {
            p = BLANC;
        }
        set_pixel_image(mask, x, y, p);
    }

    return mask;
}


Contour contour(Image I, Mask mask) {
    assert(I.L == mask.L && I.H == mask.H);

    Point start;
    if (contour_trouver_pixel_noir(mask, &start)) {
        // Coin haut-gauche du pixel
        start = add_point(start, set_point(-1, -1));

        // Accumulateur pour le contour
        ListePoints acc = liste_points_new();

        Point current_position = start;
        Orientation current_orientation = Est;

        while (true) {
            liste_points_push(&acc, current_position);

            if (current_orientation == Est) {
                set_pixel_image(mask, current_position.x + 1, current_position.y + 1, BLANC);
            }
            contour_avancer(&current_position, current_orientation);
            current_orientation = contour_nouvelle_orientation(I, current_position, current_orientation);

            if ((current_position.x == start.x) && (current_position.y == start.y) && (current_orientation == Est)) {
                liste_points_push(&acc, current_position);
                return acc;
            }
        }
    } else {
        // Contour vide = "pas de contour trouvé"
        return liste_points_new();
    }
}
