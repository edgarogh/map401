#include <stdio.h>
#include "geom2d.h"
#include "sortie.h"


void sortie_ecrire_contour(char* path, int w, int h, TableauPoints contour, SortieMode mode) {
    FILE* out = fopen(path, "w");

    fprintf(out, "%%!PS-Adobe-3.0 EPSF-3.0\n");
    fprintf(out, "%%%%BoundingBox: 0 0 %d %d\n", w, h);
    fprintf(out, "\n%% Fichier généré automatiquement, ne pas modifier. (%s)\n\n", path);

    if (mode == MODE_STROKED_POINTS) {
        fprintf(out, "%% Macro pour dessiner des points\n/circle {0.2 0 360 arc closepath 0 0 1 setrgbcolor fill} def\n\n");
    }

    Point p;
    if (contour.len > 0) {
        p = contour.inner[0];
        fprintf(out, "%f %f moveto\n", p.x, h - p.y);

        for (int i = 1; i < contour.len; i++) {
            p = contour.inner[i];
            fprintf(out, "%f %f lineto\n", p.x, h - p.y);
        }

        if (mode != MODE_FILLED) {
            fprintf(out, "0.1 setlinewidth stroke\n");
        } else {
            fprintf(out, "fill\n");
        }
    }

    if (mode == MODE_STROKED_POINTS) {
        // On commence à 1, le premier point et le dernier points sont les mêmes
        for (int i = 1; i < contour.len; i++) {
            p = contour.inner[i];
            fprintf(out, "%f %f circle\n", p.x, h - p.y);
        }
    }

    fprintf(out, "showpage\n");
    fclose(out);
}
