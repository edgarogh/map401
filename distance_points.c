#include <stdlib.h>
#include <stdio.h>
#include "geom2d.h"

int main(int argc, char** argv) {
    if (argc != 7) {
        fprintf(stderr, "utilisation: %s <Ax> <Av> <Bx> <By> <Px> <Py>\n  renvoie la distance entre le point P et le segment [AB]\n", argv[0]);
        return 1;
    }

    Point a = { atof(argv[1]), atof(argv[2]) };
    Point b = { atof(argv[3]), atof(argv[4]) };
    Point p = { atof(argv[5]), atof(argv[6]) };

    double distance = distance_point_segment(a, b, p);

    printf("%lf\n", distance);
    return 0;
}
