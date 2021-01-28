#include <assert.h>
#include <string.h>
#include "contour.h"
#include "image.h"
#include "sortie.h"


// clang (LLVM) est capable de remplacer `strlen` sur des chaînes constantes en la valeur de retour, à la compilation
static const char* const CONTOURS_EXT = ".contours";
static const char* const POSTSCRIPT_EXT = ".eps";


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


// Point d'entrée de l'utilitaire "contour_of", qui à partir d'un fichier PBM, écrit un fichier de contour du même nom +
// ".contours", et écrit des informations dans la console qui seront concaténées dans `resultats-tache2-3.txt`.
int main(int argc, char** argv) {
    assert(argc == 2);

    char* image_name = argv[1];
    Image i = lire_fichier_image(image_name);
    Contour c = contour(i);
    TableauPoints c_tab = liste_points_to_tableau_points(c);
    liste_points_supprimer(&c);

    int image_name_len = strlen(image_name);

    // Création du fichier de contour

    char* contour_file_name = concat_extension(image_name, image_name_len, CONTOURS_EXT); // Nom du fichier
    FILE* contour_file = fopen(contour_file_name, "w");
    tableau_points_enregistrer(&c_tab, contour_file);
    fclose(contour_file);
    printf("Contour %s enregistré à partir de %s (%dx%d): %d segments\n", contour_file_name, image_name, i.L, i.H, c_tab.len - 1);
    free(contour_file_name);

    // Création du fichier PostScript avec le contour

    char* ps_file_name = concat_extension(image_name, image_name_len, POSTSCRIPT_EXT); // Nom du fichier
    sortie_ecrire_contour(ps_file_name, i.L, i.H, c_tab, MODE_FILLED);
    free(ps_file_name);

    // Mémoire

    supprimer_image(&i);
    tableau_points_supprimer(&c_tab);
}
