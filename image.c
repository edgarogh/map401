/****************************************************************************** 
  Implémentation du module image_pbm
******************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"types_macros.h"
#include"image.h"

/* macro donnant l'indice d'un pixel de coordonnées (_x,_y) de l'image _I */
#define INDICE_PIXEL(_I,_x,_y) ((_x)-1)+(_I).L*((_y)-1)

/* création d'une image PBM de dimensions L x H avec tous les pixels blancs */
Image creer_image(UINT L, UINT H) {
	Image I;
	UINT i;
	
	I.L = L;
	I.H = H;
	
	/* alloctaion dynamique d'un tableau de L*H Pixel*/
	I.tab = (Pixel *)malloc(sizeof(Pixel)*L*H);
	
	/* test si le tableau a ete correctement alloué */
	if (I.tab == (Pixel *)NULL)
	{
		ERREUR_FATALE("Impossible de creer une image");
	}
	
	/* remplir le tableau avec des pixels blancs */
	for (i=0; i<L*H; i++)
		I.tab[i] = BLANC;
		
	return I;
}

/* suppression de l'image I = *p_I*/
void supprimer_image(Image *p_I) {
	free(p_I->tab);
	p_I->L = 0;
	p_I->H = 0;
}

/* renvoie la valeur du pixel (x,y) de l'image I
   si (x,y) est hors de l'image la fonction renvoie BLANC */
Pixel get_pixel_image(Image I, int x, int y) {
	if (x<1 || x>I.L || y<1 || y>I.H)
		return BLANC;
	return I.tab[INDICE_PIXEL(I,x,y)];
}

/* change la valeur du pixel (x,y) de l'image I avec la valeur v
   si (x,y) est hors de l'image la fonction ne fait rien */
void set_pixel_image(Image I, int x, int y, Pixel v) {
	if (x<1 || x>I.L || y<1 || y>I.H)
		return;
	I.tab[INDICE_PIXEL(I,x,y)] = v;
}

/* renvoie la largeur de l'image I */
UINT largeur_image(Image I) {
	return I.L;
}

/* renvoie la hauteur de l'image I */
UINT hauteur_image(Image I) {
	return I.H;
}

/* teste si le fichier d'identificateur f debute par un en-tete
   valide pour un fichier PBM :
   - ligne 1 : P1
   - suivie de zero, une ou plusieurs lignes commençant toutes par #
   La fonction se termine correctement si le fichier est correct, 
   et le pointeur de fichier se trouve à la suite l'entete.
   Sinon, l'exécution du programme est arretee avec l'affichage d'un message
   d'erreur (appel à ERREUR_FATALE)
    */ 
void entete_fichier_pbm(FILE *f) {
	char *ligne;
	size_t n;
	ssize_t l_ligne;

	/* se positionner en debut de fichier */
	fseek(f, 0, SEEK_SET);
	
	/* lecture et test de la ligne 1 */
	ligne = (char *)NULL;
	n = 0;
	l_ligne = getline(&ligne, &n, f);
	
	/* la ligne est correcte si et ssi ligne = {'P','1',0} 
	  soit une chaine de 3 caracteres (le dernier est le caractere nul) */
	if (l_ligne != 3)
	{
		ERREUR_FATALE("entete_fichier_pbm : ligne 1 incorrecte\n");
	}
	if ((ligne[0] != 'P') || (ligne[1] != '1'))
	{
		ERREUR_FATALE("entete_fichier_pbm : ligne 1 incorrecte\n");
	}
	free(ligne);
	
	/* lecture des �ventuelles lignes commençant par # */
	bool boucle_ligne_commentaire = true;
	do
	{
		/* tester d'abord la fin de fichier */
		if (feof(f))
		{
			ERREUR_FATALE("entete_fichier_pbm : fin fichier inattendue\n");
		}
		
		/* lire un caractere et tester par rapport à '#' */
		char c;
		fscanf(f, "%c", &c);
		if (c=='#')
		{
			/* lire le reste de la ligne */
			ligne = (char *)NULL;
			n = 0;
			l_ligne = getline(&ligne, &n, f);
			free(ligne);
		}
		else
		{
			/* reculer d'un caractère dans f */
			fseek(f, -1, SEEK_CUR);
			boucle_ligne_commentaire = false;
		}
	} while (boucle_ligne_commentaire);
	
}

Pixel lire_fichier_image_couleur(FILE* f) {
	char pixel;
	while (true) {
		if (fscanf(f, "%c", &pixel)) {
			if (pixel == '0') return BLANC;
			if (pixel == '1') return NOIR;
			continue;
		} else {
			// Impossible de lire un caractère. On a probablement atteint l'EOF
			ERREUR_FATALE("lire_fichier_image_couleur : impossible de lire une couleur\n");
		}
	}
}

/* lire l'image dans le fichier nommé nom_f
   s'il y a une erreur dans le fichier le programme s'arrete en affichant
   un message */
Image lire_fichier_image(char *nom_f) {
	FILE *f;
	UINT L, H;
	
	/* ouverture du fichier nom_f en lecture */
	f = fopen(nom_f, "r");
	if (f == (FILE *)NULL) {
	    fprintf(stderr, "lire_fichier_image : ouverture du fichier impossible: %s\n", nom_f);
        exit(1);
	}
	
	/* traitement de l'en-tete et arret en cas d'erreur */
	entete_fichier_pbm(f);
	
	/* lecture des dimensions - lecture de 2 entiers */
	if (!fscanf(f, "%d %d", &L, &H)) {
		ERREUR_FATALE("lire_fichier_image : impossible de lire les dimensions de l'image\n");
	}
	
	/* création de l'image I de dimensions L x H */
	Image I = creer_image(L, H);
	
	/* lecture des pixels du fichier - lecture caractère par caractère
	   seuls les caracteres '0' (BLANC) ou '1' (NOIR) 
	   doivent etre pris en compte */

	FOR_EACH_PIXEL(I, x, y) {
		set_pixel_image(I, x, y, lire_fichier_image_couleur(f));
	}

	/* fermeture du fichier */
	fclose(f);
	
	return I;
}

/* écrire l'image I à l'écran */
void ecrire_image(Image I) {
    printf("%dx%d\n", I.L, I.H);

	for (UINT y = 1; y <= I.H; y++) {
		for (UINT x = 1; x <= I.L; x++) {
			fputc((get_pixel_image(I, x, y) == BLANC) ? '#' : ' ', stdout);
		}
        fputc('\n', stdout);
	}
}
