# MAP401

Projet de vectorisation d'images PBM au format EPS réalisé seul dans le contexte de l'UE de MAP401 à l'UGA.

(PBM to EPS image vectorization project done alone as part of the "MAP401" teaching unit at the Université Grenoble Alpes)

## Téléchargement et compilation

Prérequis:
  * Un ordinateur
  * Un système d'exploitation sympa (je recommande [Linux Mint](https://linuxmint.com/) et déconseille Windows et [AnanOS](https://github.com/WartaPoirier-corp/ananos/))
  * `clang` (facilement adaptable à un autre compilateur C)
  * GNU Make
  * (optionnel, pour la gestion des versions) git
  * (optionnel, ne vous servira strictement à rien pour utiliser ce projet) `rustc` parce que Rust est le meilleur langage de programmation du monde et que vous devriez le pratiquer :crab:

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
    -c    Sort un fichier de contours <image.pbm>.contours
    -1    Sort un fichier EPS <image.pbm>-mode1.eps dans le mode de rendu 1
    -2     …
    -3     …

EXEMPLES:
    contour_of images/coq.pbm -c -3
    contour_of images/coq.pbm -1
```

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
