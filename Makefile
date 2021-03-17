#############################################################################
# Fichier Makefile 
# UE MAP401 - DLST - UGA - 2020/2021
#############################################################################
# utilisation des variables internes $@ $< $^ $*
# $@ : correspond au nom de la cible
# $< : correspond au nom de la premiere dependance
# $^ : correspond � toutes les d�pendances
# $* : correspond au nom du fichier sans extension 
#       (dans les regles generiques uniquement)
#############################################################################
# information sur la regle executee avec la commande @echo
# (une commande commancant par @ n'est pas affichee a l'ecran)
#############################################################################


#############################################################################
# definition des variables locales
#############################################################################

# compilateur C
CC = clang

# chemin d'acces aux librairies (interfaces)
INCDIR = .

# chemin d'acces aux librairies (binaires)
LIBDIR = .

# options pour l'�dition des liens
LDOPTS = -L$(LIBDIR) -lm

# options pour la recherche des fichiers .o et .h
INCLUDEOPTS = -I$(INCDIR)

# options de compilation
COMPILOPTS = -g -Wall $(INCLUDEOPTS)

# liste des executables
EXECUTABLES = test_image test_geom2d test_contour contour_of distance_points


#############################################################################
# definition des regles
#############################################################################

########################################################
# la règle par défaut
all : $(EXECUTABLES)

########################################################
# listes génériques
liste_%.c liste_%.h: listes/%.env listes/liste_.h listes/liste_.c Makefile
	{ \
	. $< ;\
	replace="sed -e s/#INCLUDE_TEMPLATE/$${INCLUDE}/g -e s/_LISTE_TTTTT_H_/_LISTE_$${UP}_H_/g -e s/TTTTT/$${CAP}/g -e s/ttttt/$${LOW}/g" ;\
	$$replace listes/liste_.h > liste_$*.h ;\
	$$replace listes/liste_.c > liste_$*.c ;\
	}

########################################################
# regle generique : 
#  remplace les regles de compilation separee de la forme
#	module.o : module.c module.h
#		$(CC) -c $(COMPILOPTS) module.c
%.o : %.c %.h liste_point.h liste_bezier2.h
	@echo ""
	@echo "---------------------------------------------"
	@echo "Compilation du module "$*
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

########################################################
# regles explicites de compilation separee de modules
# n'ayant pas de fichier .h ET/OU dependant d'autres modules
image.o : image.c image.h types_macros.h
	@echo ""
	@echo "---------------------------------------------"
	@echo "Compilation du module image"
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

test_image.o : test_image.c image.h 
	@echo ""
	@echo "---------------------------------------------"
	@echo "Compilation du module test_image"
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

contour.o: contour.c contour.h geom2d.h liste_point.h tableau_points.h
	@echo ""
	@echo "---------------------------------------------"
	@echo "Compilation du module contour"
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

liste_points.o: liste_points.c liste_point.h geom2d.h
	@echo ""
	@echo "---------------------------------------------"
	@echo "Compilation du module liste_points"
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

tableau_points.o: tableau_points.c liste_point.h tableau_points.h geom2d.h
	@echo ""
	@echo "---------------------------------------------"
	@echo "Compilation du module tableau_points"
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $<

sortie.o: sortie_eps.c sortie.h contour.h liste_point.h tableau_points.h
	@echo ""
	@echo "---------------------------------------------"
	@echo "Compilation du module sortie_eps"
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $< -o $@

simplification.o: simplification.c simplification.h tableau_points.h geom2d.h liste_point.h liste_bezier2.h
	@echo ""
	@echo "---------------------------------------------"
	@echo "Compilation du module simplification"
	@echo "---------------------------------------------"
	$(CC) -c $(COMPILOPTS) $< -o $@

########################################################
# regles explicites de creation des executables

test_image : test_image.o image.o
	@echo ""
	@echo "---------------------------------------------"
	@echo "Creation de l'executable "$@
	@echo "---------------------------------------------"
	$(CC) $^ $(LDOPTS) -o $@

test_geom2d: test_geom2d.o geom2d.o
	@echo ""
	@echo "---------------------------------------------"
	@echo "Creation de l'executable "$@
	@echo "---------------------------------------------"
	$(CC) $^ $(LDOPTS) -o $@

test_contour: test_contour.o image.o contour.o liste_point.o tableau_points.o geom2d.o
	@echo ""
	@echo "---------------------------------------------"
	@echo "Creation de l'executable "$@
	@echo "---------------------------------------------"
	$(CC) $^ $(LDOPTS) -o $@

contour_of: contour_of.o image.o contour.o liste_point.o tableau_points.o geom2d.o sortie.o simplification.o
	@echo ""
	@echo "---------------------------------------------"
	@echo "Creation de l'executable "$@
	@echo "---------------------------------------------"
	$(CC) $^ $(LDOPTS) -o $@

distance_points: distance_points.c geom2d.o
	@echo ""
	@echo "---------------------------------------------"
	@echo "Creation de l'executable "$@
	@echo "---------------------------------------------"
	$(CC) $^ $(LDOPTS) -o $@

# regle pour "nettoyer" le répertoire
clean:
	rm -fR $(EXECUTABLES) *.o images/*.pbm.contours liste_*.c liste_*.h

test: test_image test_geom2d test_contour
	./test_geom2d && ./test_image && ./test_contour

contours: contour_of
	ls -1 images/*.pbm | xargs -L1 -I{} ./contour_of {} -c -3 > resultats-tache3-2.txt
	./contour_of images/chat.pbm -1 -2
	./contour_of images/image_ex_poly.pbm -1 -2
	./contour_of images/image2_poly.pbm -1 -2
