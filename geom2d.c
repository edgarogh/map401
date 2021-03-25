#include <assert.h>
#include <math.h>
#include "geom2d.h"

#define DOUBLE_EPSILON 0.0000001


bool egaux_doubles(double d1, double d2) {
    return fabs(d1 - d2) < DOUBLE_EPSILON;
}


// Points


Point set_point(double x, double y) {
    return (Point) { x, y };
}


Point add_point(Point p1, Point p2) {
    return set_point(p1.x + p2.x, p1.y + p2.y);
}


Point mul_reel_point(Point p, double a) {
    return set_point(a * p.x, a * p.y);
}


double distance(Point p1, Point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}


bool egaux_points(Point p1, Point p2) {
    return egaux_doubles(p1.x, p2.x) && egaux_doubles(p1.y, p2.y);
}


// Vecteurs


Vecteur set_vecteur(double x, double y) {
    return (Vecteur) { x, y };
}


Vecteur add_vecteur(Vecteur v1, Vecteur v2) {
    return set_vecteur(v1.x + v2.x, v1.y + v2.y);
}


Vecteur mul_reel_vecteur(Vecteur v, double a) {
    return set_vecteur(a * v.x, a * v.y);
}


Vecteur vect_bipoint(Point a, Point b) {
    return set_vecteur(b.x - a.x, b.y - a.y);
}


double produit_scalaire(Vecteur v1, Vecteur v2) {
    return (v1.x * v2.x) + (v1.y * v2.y);
}


double norme(Vecteur v) {
    return sqrt(pow(v.x, 2) + pow(v.y, 2));
}


bool egaux_vecteurs(Vecteur v1, Vecteur v2) {
    return egaux_doubles(v1.x, v2.x) && egaux_doubles(v1.y, v2.y);
}


// Distance point-segment


double distance_point_segment(Point a, Point b, Point p) {
    if (egaux_points(a, b)) return distance(a, p);

    Vecteur ab = vect_bipoint(a, b);
    double lambda = produit_scalaire(vect_bipoint(a, p), ab) / produit_scalaire(ab, ab);

    if (lambda < 0) {
        return distance(a, p);
    } else if (lambda > 1) {
        return distance(b, p);
    } else {
        Point q = add_point(a, mul_reel_point(add_point(b, mul_reel_point(a, -1)), lambda));
        return distance(q, p);
    }
}


// Courbes de bezier


Point bezier2_C(Bezier2* self, double t) {
    Point m0 = mul_reel_point(self->c0, pow(1 - t, 2));
    Point m1 = mul_reel_point(self->c1, 2 * t * (1 - t));
    Point m2 = mul_reel_point(self->c2, t * t);

    return add_point(m0, add_point(m1, m2));
}


double distance_point_bezier2(Bezier2* self, Point p, double t) {
    return distance(bezier2_C(self, t), p);
}


Point bezier3_C(Bezier3* self, double t) {
    Point m0 = mul_reel_point(self->c0, pow(1 - t, 3));
    Point m1 = mul_reel_point(self->c1, 3 * t * pow(1 - t, 2));
    Point m2 = mul_reel_point(self->c2, 3 * t * t * (1 - t));
    Point m3 = mul_reel_point(self->c3, t * t * t);

    return add_point(m0, add_point(m1, add_point(m2, m3)));
}


double distance_point_bezier3(Bezier3* self, Point p, double t) {
    return distance(bezier3_C(self, t), p);
}


Bezier3 bezier2_to_bezier3(Bezier2* self) {
    return (Bezier3) {
        self->c0,
        mul_reel_point(add_point(self->c0, mul_reel_point(self->c1, 2)), 1./3.),
        mul_reel_point(add_point(self->c2, mul_reel_point(self->c1, 2)), 1./3.),
        self->c2,
    };
}


Bezier2 approx_bezier2(Point *start, unsigned int len) {
    assert(len > 1);

    Bezier2 curve = {
            .c0 = start[0],
    };

    if (len == 2) {
        curve.c1 = mul_reel_point(add_point(start[0], start[1]), .5);
        curve.c2 = start[1];
    } else {
        double n = len - 1;

        Point sum = set_point(0, 0);
        for (int i = 1; i < (len-1); i++) {
            sum = add_point(sum, start[i]);
        }

        double alpha = (3. * n) / (n*n - 1.);
        double beta = (1. - (2. * n)) / (2. * (n + 1.));

        curve.c2 = start[len-1];
        curve.c1 = add_point(mul_reel_point(sum, alpha), mul_reel_point(add_point(curve.c0, curve.c2), beta));
    }

    return curve;
}


double approx_bezier3_gamma(double k, double n) {
    return (6. * pow(k, 4.)) - (8. * n * pow(k, 3.)) + (6. * k * k) - (4. * n * k) + pow(n, 4.) - (n * n);
}


Bezier3 approx_bezier3(Point* start, unsigned int len) {
    assert(len > 1);

    if (len <= 3) {
        Bezier2 b2 = approx_bezier2(start, len);
        return bezier2_to_bezier3(&b2);
    } else {
        Bezier3 curve = {
                .c0 = start[0],
                .c3 = start[len-1],
        };

        double n = len - 1;

        Point sum1 = set_point(0, 0);
        for (int i = 1; i < (len-1); i++) {
            sum1 = add_point(sum1, mul_reel_point(start[i], approx_bezier3_gamma(i, n)));
        }

        Point sum2 = set_point(0, 0);
        for (int i = 1; i < (len-1); i++) {
            sum2 = add_point(sum2, mul_reel_point(start[i], approx_bezier3_gamma(n - (double) i, n)));
        }

        double d = 3. * (n + 2.) * (3. * n * n + 1.);
        double alpha = ((-15. * n * n * n) + (5. * n * n) + (2. * n) + 4.) / d;
        double beta = ((10. * n * n * n) - (15. * n * n) + n + 2.) / d;
        double lambda = (70. * n) / (3. * (n * n - 1.) * (n * n - 4.) * (3. * n * n + 1.));

        curve.c1 = add_point(mul_reel_point(curve.c0, alpha), add_point(mul_reel_point(sum1, lambda), mul_reel_point(curve.c3, beta)));
        curve.c2 = add_point(mul_reel_point(curve.c0, beta), add_point(mul_reel_point(sum2, lambda), mul_reel_point(curve.c3, alpha)));

        return curve;
    }
}
