typedef struct Vecteur_ {
    double x, y;
} Vecteur;

typedef struct Point_ {
    double x, y;
} Point;

Point set_point(double x, double y);

Point add_point(Point p1, Point p2);

Vecteur vect_bipoint(Point a, Point b);
