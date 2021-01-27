#include <assert.h>
#include <string.h>
#include "contour.h"
#include "image.h"

// Point d'entrée de l'utilitaire "contour_of", qui à partir d'un fichier PBM, écrit un fichier de contour du même nom +
// ".contours", et écrit des informations dans la console qui seront concaténées dans `resultats-tache2-3.txt`.
int main(int argc, char** argv) {
    assert(argc == 2);

    char* image_name = argv[1];
    Image i = lire_fichier_image(image_name);
    Contour c = contour(i);
    TableauPoints c_tab = liste_points_to_tableau_points(c);

    // Nom du fichier de sortie

    int image_name_len = strlen(image_name);
    // On alloue la taille du nom du fichier d'entrée, plus la taille de la chaîne ".contour" avec le \0 final
    char* contour_file_name = malloc(image_name_len + 10);
    // Les deux lignes suivantes construisent le nom du fichier de sortie partie par partie
    strcpy(contour_file_name, image_name);
    strcpy(&contour_file_name[image_name_len], ".contours");

    FILE* contour_file = fopen(contour_file_name, "w");
    tableau_points_enregistrer(&c_tab, contour_file);
    fclose(contour_file);
    printf("Contour %s enregistré à partir de %s (%dx%d): %d segments\n", contour_file_name, image_name, i.L, i.H, c_tab.len - 1);

    // Mémoire

    free(contour_file_name);
    liste_points_supprimer(&c);
    tableau_points_supprimer(&c_tab);
}
