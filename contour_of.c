#include <assert.h>
#include <string.h>
#include "contour.h"
#include "image.h"
#include "sortie.h"


static const char* const AIDE = "\n"
              "UTILISATION:\n"
              "    contour_of <image.pbm> [flags]\n"
              "\n"
              "FLAGS:\n"
              "    -c    Sort un fichier de contours <image.pbm>.contours\n"
              "    -1    Sort un fichier EPS <image.pbm>-mode1.eps dans le mode de rendu 1\n"
              "    -2     …\n"
              "    -3     …\n"
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


void creer_eps(Image i, TableauPoints tab, char* image_name, unsigned int image_name_len, SortieMode mode) {
    char* ps_file_name = concat_extension(image_name, image_name_len, POSTSCRIPT_EXT[mode]); // Nom du fichier
    sortie_ecrire_contour(ps_file_name, i.L, i.H, tab, mode);
    free(ps_file_name);
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

    char* image_name = argv[1];
    bool f_contour = false, f_mode1 = false, f_mode2 = false, f_mode3 = false; // flags

    // On itère sur les arguments restants
    argc--;
    for (argv = &argv[2]; argc-- > 1; argv = &argv[1]) {
        char* arg = argv[0];

        if (arg[0] != '-' || arg[1] == 0 || arg[2] != '\0') {
            return flag_invalide(arg);
        }

        switch (arg[1]) {
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

    Image i = lire_fichier_image(image_name);
    Contour c = contour(i);
    TableauPoints c_tab = liste_points_to_tableau_points(c);
    liste_points_supprimer(&c);

    int image_name_len = strlen(image_name);

    // Création du fichier de contour

    if (f_contour) {
        char* contour_file_name = concat_extension(image_name, image_name_len, CONTOURS_EXT); // Nom du fichier
        FILE* contour_file = fopen(contour_file_name, "w");
        tableau_points_enregistrer(&c_tab, contour_file);
        fclose(contour_file);
        printf("Contour %s enregistré à partir de %s (%dx%d): %d segments\n", contour_file_name, image_name, i.L, i.H, c_tab.len - 1);
        free(contour_file_name);
    }

    // Création du fichier PostScript avec le contour

    if (f_mode1) creer_eps(i, c_tab, image_name, image_name_len, MODE_STROKED);
    if (f_mode2) creer_eps(i, c_tab, image_name, image_name_len, MODE_STROKED_POINTS);
    if (f_mode3) creer_eps(i, c_tab, image_name, image_name_len, MODE_FILLED);

    // Mémoire

    supprimer_image(&i);
    tableau_points_supprimer(&c_tab);
}
