# MAP401 -- Instructions

## Téléchargement et compilation

Prérequis:
  * `clang` (facilement adaptable à un autre compilateur C)
  * GNU Make
  * (optionnel, pour la gestion des versions) git

Clonage du dépôt:

```
git clone https://github.com/edgarogh/map401 map401-edgarogh
cd map401-edgarogh/
```

Basculer sur une version spécifique (optionnel):

  * `git tag --list` pour lister les étiquettes de versions
  * `git checkout <rev>` où `<rev>` est le nom d'une étiquette

Compilation:

```
make
```

## Utilisation

La commande principale est `contour_of <image.pbm> [drapeaux]`. La commande prend en argument une image PBM et créée, selon les drapeaux définis, des fichiers de contour ou des fichiers EPS avec différentes options. Le menu d'aide affiché ci-dessous peut être obtenu en tapant simplement `./contour_of` sans aucun argument.

```
❯ ./contour_of

UTILISATION:
    contour_of <image.pbm> [flags]

FLAGS:
    === traitement préalable (0 ou 1 choix possible) ===============================
    -d <d>    Simplifie le contour avec l'algo. de Douglas-Peucker pour un seuil <d>
    -b <d>    ↳ + simplification par Bezier quadratique
    -B <d>    ↳ + simplification par Bezier cubique
    === sortie (plusieurs choix possibles) =========================================
    -c        Sort un fichier de contours <image.pbm>.contours
    -1        Sort un fichier EPS <image.pbm>-mode1.eps dans le mode de rendu 1
    -2        "                                                               2
    -3        "                                                               3

EXEMPLES:
    contour_of images/coq.pbm -c -3
    contour_of images/coq.pbm -1
```

En résumé: après avoir spécifié le chemin vers l'image, on peut choisir zéro ou un mode de traitement (simplification), et spécifier autant de modes de sortie que l'on veut. Le mode de sortie `-c` permet aussi d'afficher des informations sur le nombre de segments, de courbes, et de contours dans la console.

## Exécution des tests

`make test`

## License

Les droits des fichiers ci-dessous ne me sont pas concédés sous une license particulière, et les doivent être considérés comme réservés par leurs auteur·ice·s respectif·ve·s:
  * `image.c`, `image.h`
  * `test_image.c`
  * `Makefile`
  * `types_macros.h`
  * Tous les fichiers `.pbm` du répertoire `images/`

Je suis l'auteur de tous les autres fichiers et les place sous license GPL-3.0, dont un exemplaire est fourni dans le dépôt (`/gpl-3.0.txt`).

---

Requêtes, réclamations et contact général: dev (AT) edgar.bzh
