#include "geom2d.h"
#include "sortie.h"


FichierSortie sortie_open(char* path, int w, int h, SortieMode mode) {
    FILE* out = fopen(path, "w");

    fprintf(out, "%%!PS-Adobe-3.0 EPSF-3.0\n");
    fprintf(out, "%%%%BoundingBox: 0 0 %d %d\n", w, h);
    fprintf(out, "\n%% Fichier généré automatiquement, ne pas modifier. (%s)\n\n", path);

    if (mode == MODE_STROKED_POINTS) {
        fprintf(out, "%% Macro pour dessiner des points\n/circle {0.2 0 360 arc closepath 0 0 1 setrgbcolor fill} def\n\n");
    }

    return (FichierSortie) {
        .out = out,
        .mode = mode,
        .h = h,
    };
}


void sortie_ecrire_contour(FichierSortie self, TableauPoints contour) {
    FILE* out = self.out;
    SortieMode mode = self.mode;

    Point p;
    if (contour.len > 0) {
        p = contour.inner[0];
        fprintf(out, "%f %f moveto\n", p.x, self.h - p.y);

        for (int i = 1; i < contour.len; i++) {
            p = contour.inner[i];
            fprintf(out, "%f %f lineto\n", p.x, self.h - p.y);
        }

        if (mode != MODE_FILLED) {
            fprintf(out, "0.1 setlinewidth stroke\n");
        }
    }

    if (mode == MODE_STROKED_POINTS) {
        // On commence à 1, le premier point et le dernier points sont les mêmes
        for (int i = 1; i < contour.len; i++) {
            p = contour.inner[i];
            fprintf(out, "%f %f circle\n", p.x, self.h - p.y);
        }
    }
}


void sortie_ecrire_contour_bezier(FichierSortie self, ListeBezier3* contour) {
    FILE* out = self.out;
    SortieMode mode = self.mode;

    Point p;
    if (contour->len > 0) {
        ListeBezier3Noeud* noeud = contour->first;

        p = noeud->value.c0;
        fprintf(out, "%f %f moveto\n", p.x, self.h - p.y);

        while (noeud) {
            Point c1 = noeud->value.c1, c2 = noeud->value.c2;
            p = noeud->value.c3;
            fprintf(out, "%f %f %f %f %f %f curveto\n", c1.x, self.h - c1.y, c2.x, self.h - c2.y, p.x, self.h - p.y);

            noeud = noeud->next;
        }

        if (mode != MODE_FILLED) {
            fprintf(out, "0.1 setlinewidth stroke\n");
        }
    }

    if (mode == MODE_STROKED_POINTS) {
        // On commence à 1, le premier point et le dernier points sont les mêmes
        for (ListeBezier3Noeud* noeud = contour->first; noeud; noeud = noeud->next) {
            Point p = noeud->value.c1;
            fprintf(out, "%f %f circle\n", p.x, self.h - p.y);
        }
    }
}


void sortie_close(FichierSortie self) {
    FILE* out = self.out;

    if (self.mode == MODE_FILLED) {
        fprintf(out, "fill\n");
    }

    fprintf(out, "showpage\n");
    fclose(out);
}
