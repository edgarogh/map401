#include "geom2d.h"

Point set_point(double x, double y) {
    return (Point) { x, y };
}

Point add_point(Point p1, Point p2) {
    return set_point(p1.x + p2.x, p1.y + p2.y);
}

Vecteur vect_bipoint(Point a, Point b) {
    return (Vecteur) { b.x - a.x, b.y - a.y };
}