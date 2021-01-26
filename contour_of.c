#include <assert.h>
#include <string.h>
#include "contour.h"
#include "image.h"

int main(int argc, char** argv) {
    assert(argc == 2);

    char* image_name = argv[1];
    Image i = lire_fichier_image(image_name);
    Contour c = contour(i);
    TableauPoints c_tab = liste_points_to_tableau_points(c);

    // Nom du fichier de sortie
    int image_name_len = strlen(image_name);
    char* contour_file_name = malloc(9 + image_name_len);
    strcpy(contour_file_name, image_name);
    strcpy(&contour_file_name[image_name_len], ".contours");

    FILE* contour_file = fopen(contour_file_name, "w");
    tableau_points_enregistrer(&c_tab, contour_file);
    fclose(contour_file);
    printf("Contour %s enregistré à partir de %s: %d segments\n", contour_file_name, image_name, c_tab.len - 1);

    // Mémoire
    free(contour_file_name);
    liste_points_supprimer(&c);
    tableau_points_supprimer(&c_tab);
}
