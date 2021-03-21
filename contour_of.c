#include <assert.h>
#include <string.h>
#include "contour.h"
#include "image.h"
#include "simplification.h"
#include "sortie.h"


static const char* const AIDE = "\n"
              "UTILISATION:\n"
              "    contour_of <image.pbm> [flags]\n"
              "\n"
              "FLAGS:\n"
              "    === traitement préalable (0 ou 1 choix possible) ===============================\n"
              "    -d <d>    Simplifie le contour avec l'algo. de Douglas-Peucker pour un seuil <d>\n"
              "    -b <d>    ↳ + simplification par Bezier quadratique\n"
              "    -B <d>    ↳ + simplification par Bezier cubique\n"
              "    === sortie (plusieurs choix possibles) =========================================\n"
              "    -c        Sort un fichier de contours <image.pbm>.contours\n"
              "    -1        Sort un fichier EPS <image.pbm>-mode1.eps dans le mode de rendu 1\n"
              "    -2        \"                                                               2\n"
              "    -3        \"                                                               3\n"
              "\n"
              "EXEMPLES:\n"
              "    contour_of images/coq.pbm -c -3\n"
              "    contour_of images/coq.pbm -1\n"
              ;


// clang (LLVM) est capable de remplacer `strlen` sur des chaînes constantes en la valeur de retour, à la compilation
static const char* const CONTOURS_EXT = ".contours";
static const char* const POSTSCRIPT_EXT[] = {
        [MODE_STROKED] = "-mode1.eps",
        [MODE_STROKED_POINTS] = "-mode2.eps",
        [MODE_FILLED] = "-mode3.eps",
};


/*
 * Prend un nom de fichier et lui concatène une extension. Renvoie un pointeur `malloc`é vers la concaténation, qu'il ne
 * faut pas oublier de libérer.
 * En théorie, fonctionne avec n'importe quelles chaînes. Je spécialise sa spécification à des fichiers / extensions
 * pour plus de clarté dans le contexte actuel.
*/
char* concat_extension(char* base, unsigned int base_len, const char* const ext) {
    unsigned int ext_len = strlen(ext);

    char* file_name = malloc(base_len + ext_len + 1);
    strcpy(file_name, base);
    strcpy(&file_name[base_len], ext);
    return file_name;
}


int flag_invalide(char* f) {
    fprintf(stderr, "Flag invalide: %s\n", f);
    return 1;
}


FichierSortie creer_eps(Image i, char* image_name, unsigned int image_name_len, SortieMode mode) {
    char* ps_file_name = concat_extension(image_name, image_name_len, POSTSCRIPT_EXT[mode]); // Nom du fichier
    FichierSortie fs = sortie_open(ps_file_name, i.L, i.H, mode);
    free(ps_file_name);
    return fs;
}


/**
 * Enlève et retourne un argument de ligne de commande
 */
char* pop_arg(int* argc, char*** argv) {
    if ((*argc)--) {
        *argv = &(*argv)[1];
        return (*argv)[0];
    } else {
        return NULL;
    }
}


typedef enum {
    DPNone = 0,
    DPSegment = 1,
    DPBezier2 = 2,
    DPBezier3 = 3,
} DP;


/**
 * Représente un contour; soit constitué de segments consécutifs, soit constitué de Bezier3 consécutives
 */
typedef struct {
    DP tag;
    union {
        TableauPoints segment;
        ListeBezier3 bezier3;
    } val;
} GenericContour;


/**
 * Renvoie vrai si le contour générique est constitué de segments (et donc pas de courbes de Bezier)
 */
bool generic_contour_is_segment(GenericContour* self) {
    return self->tag <= DPSegment;
}


unsigned int generic_contour_len(GenericContour* self) {
    if (generic_contour_is_segment(self)) {
        return self->val.segment.len;
    } else {
        return self->val.bezier3.len;
    }
}


/**
 * Ecrit un contour générique dans un fichier EPS. Appelle une fonction du module `sortie` différente selon le type de
 * contour.
 */
void generic_contour_ecrire(GenericContour* self, FichierSortie out) {
    if (generic_contour_is_segment(self)) {
        sortie_ecrire_contour(out, self->val.segment);
    } else {
        sortie_ecrire_contour_bezier(out, &self->val.bezier3);
    }
}


char* generic_contour_el_name(GenericContour* self) {
    if (generic_contour_is_segment(self)) {
        return "segments";
    } else {
        return "courbes de Bezier";
    }
}


// Point d'entrée de l'utilitaire "contour_of", qui à partir d'un fichier PBM, écrit un fichier de contour du même nom +
// ".contours", et écrit des informations dans la console qui seront concaténées dans `resultats-tache2-3.txt`.
int main(int argc, char** argv) {
    if (argc == 1) {
        fprintf(stderr, "%s", AIDE);
        return 1;
    } else if (argc == 2) {
        fprintf(stderr, "Vous devez spécifier au minimum un mode de sortie (-c, -2, etc.).\nRentrez la commande sans arguments pour accéder à l'aide.\n");
        return 1;
    }

    char* image_name = pop_arg(&argc, &argv);
    double seuil_dp = .0;
    DP type_dp = DPNone;
    bool f_contour = false, f_mode1 = false, f_mode2 = false, f_mode3 = false; // flags

    // On itère sur les arguments restants
    char* arg;
    while ((arg = pop_arg(&argc, &argv))) {
        if (arg[0] != '-' || arg[1] == 0 || arg[2] != '\0') {
            return flag_invalide(arg);
        }

        switch (arg[1]) {
            case 'B':
                type_dp++;
                // cascade volontaire
            case 'b':
                type_dp++;
                // cascade volontaire
            case 'd': {
                type_dp++;
                char* seuil_db_str = pop_arg(&argc, &argv);
                char* end;
                seuil_dp = strtod(seuil_db_str, &end);
                if (seuil_db_str == end) {
                    fprintf(stderr, "Le seuil n'a pas pu être décodé. Assurez vous de le spécifier sous la forme suivante: `… -d 12 …`\n");
                    return 1;
                }
                break;
            }
            case 'c':
                f_contour = true;
                break;
            case '1':
                f_mode1 = true;
                break;
            case '2':
                f_mode2 = true;
                break;
            case '3':
                f_mode3 = true;
                break;
            default:
                return flag_invalide(arg);
        }
    }

    if (type_dp == DPBezier3) {
        ERREUR_FATALE("Bezier3 non implémenté");
    }

    Image i = lire_fichier_image(image_name);
    Mask mask = contour_init_mask(i);

    int image_name_len = strlen(image_name);

    // === Ouverture des fichiers ===

    FILE *contour_file = NULL;
    if (f_contour) {
        char *contour_file_name = concat_extension(image_name, image_name_len, CONTOURS_EXT); // Nom du fichier
        contour_file = fopen(contour_file_name, "w");
        // J'"alloue" de l'espace au début du fichier, car j'aurais besoin d'y écrire le nombre de contours que je ne
        // connaîtrait qu'à la fin (avec `rewind`). Si je ne fais pas ça, les `fprintf` suivant `rewind` vont écraser
        // les caractères existants.
        fprintf(contour_file, "            "); // un u32 fait 11 caractères max + les sauts de ligne
        free(contour_file_name);
    }

    FichierSortie eps1_file;
    if (f_mode1) eps1_file = creer_eps(i, image_name, image_name_len, MODE_STROKED);
    FichierSortie eps2_file;
    if (f_mode2) eps2_file = creer_eps(i, image_name, image_name_len, MODE_STROKED_POINTS);
    FichierSortie eps3_file;
    if (f_mode3) eps3_file = creer_eps(i, image_name, image_name_len, MODE_FILLED);

    // === Itération sur les contours ===

    // Premier contour
    Contour c = contour(i, mask);
    if (c.len == 0) { // Rappel: un contour de taille zero signifie "pas de contour trouvé"
        fprintf(stderr, "avertissement: aucun contour trouvé dans le fichier fourni\n");
    }

    unsigned int segments = 0; // pour les statistiques affichées dans stdout
    unsigned int contours = 0;
    char* el_name = NULL; // "segments" ou "courbes de Bezier"
    while (c.len != 0) {
        contours++;

        TableauPoints c_tab = liste_point_to_tableau_points(c);
        liste_point_supprimer(&c);

        // c_tab, simplifié avec l'algo D.-P. ou tel quel (selon si une simplification est demandée ou non). Cette
        // variable sert uniquement à générer le fichier de contour, elle n'est définie que si besoin.
        //   - aucune simplification demandée: c_tab_dp = c_tab
        //   - simplification simple par segment: c_tab_dp = *la version simplifiée*
        //   - simplification par bezier: c_tab_dp = *indéfini* si le fichier de contour n'est pas demandé,
        //     *simplification simple par segments* sinon
        TableauPoints c_tab_dp = (TableauPoints) { .len = 0 };
        // Contour générique. Il peut s'agir d'un `TableauPoints` OU d'une `ListeBezier3`
        GenericContour generic_contour;
        // Défini lorsqu'on utilise des courbes de Bezier uniquement, utilisé pour libérer la mémoire à la fin
        ListeBezier3 cb = { .len = 0 };
        switch (type_dp) {
            case DPNone: {
                generic_contour = (GenericContour) {
                    .tag = DPSegment,
                    .val = { .segment = c_tab },
                };
                c_tab_dp = c_tab;
                break;
            }
            case DPSegment: {
                c = simplification_douglas_peucker(c_tab, 0, c_tab.len - 1, seuil_dp);
                c_tab_dp = liste_point_to_tableau_points(c);
                generic_contour = (GenericContour) {
                    .tag = DPSegment,
                    .val = { .segment = c_tab_dp },
                };
                liste_point_supprimer(&c);
                break;
            }
            case DPBezier2:
            case DPBezier3: {
                // TODO gérer Bezier3
                cb = simplification_douglas_peucker_bezier2(c_tab, 0, c_tab.len - 1, seuil_dp);
                generic_contour = (GenericContour) {
                    .tag = DPBezier3,
                    .val = { .bezier3 = cb },
                };
                if (contour_file) {
                    c = simplification_douglas_peucker(c_tab, 0, c_tab.len - 1, seuil_dp);
                    c_tab_dp = liste_point_to_tableau_points(c);
                    liste_point_supprimer(&c);
                }
                break;
            }
        }

        // Ajout du contour du fichier de contour
        if (f_contour) {
            tableau_points_enregistrer(&c_tab_dp, contour_file);
        }

        if (!el_name) {
            el_name = generic_contour_el_name(&generic_contour);
        }

        if (f_mode1) generic_contour_ecrire(&generic_contour, eps1_file);
        if (f_mode2) generic_contour_ecrire(&generic_contour, eps2_file);
        if (f_mode3) generic_contour_ecrire(&generic_contour, eps3_file);

        segments += (generic_contour_len(&generic_contour) - 1);

        if (cb.len > 0) liste_bezier3_supprimer(&cb);
        if (c_tab_dp.len > 0 && c_tab_dp.inner != c_tab.inner) tableau_points_supprimer(&c_tab_dp);
        tableau_points_supprimer(&c_tab);
        c = contour(i, mask);
    }

    // === Ecriture du # de contours et de la BB maintenant qu'on connait tous les contours ===

    if (contour_file) {
        // On revient au début du fichier pour écrire le nombre de contours
        rewind(contour_file);
        fprintf(contour_file, "%d\n", contours);

        fclose(contour_file);

        char* pluriel = contours == 1 ? "" : "s";
        printf("%d contour%s de %s enregistré%s (%dx%d): %d %s\n",
               contours,
               pluriel,
               image_name,
               pluriel,
               i.L,
               i.H,
               segments,
               el_name
               );
    }

    if (f_mode1) sortie_close(eps1_file);
    if (f_mode2) sortie_close(eps2_file);
    if (f_mode3) sortie_close(eps3_file);

    // === Libération des resources ===

    supprimer_image(&mask);
    supprimer_image(&i);
}
