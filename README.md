# CoinLang
Compilateur d'un mini-langage

[![GitHub release](https://img.shields.io/github/release/TristanEduProjet/CoinLang.svg)](https://github.com/TristanEduProjet/CoinLang/releases)
[![GitHub Releases](https://img.shields.io/github/downloads/TristanEduProjet/CoinLang/latest/total.svg)](https://github.com/TristanEduProjet/CoinLang/releases)
[![GitHub All Releases](https://img.shields.io/github/downloads/TristanEduProjet/CoinLang/total.svg)](https://github.com/TristanEduProjet/CoinLang/releases)  
Unix : [![CircleCI](https://circleci.com/gh/TristanEduProjet/CoinLang.svg?style=svg)](https://circleci.com/gh/TristanEduProjet/CoinLang)
Windows : [![Build status](https://ci.appveyor.com/api/projects/status/c62tf4yj8a6i41sf/branch/master?svg=true)](https://ci.appveyor.com/project/TristanEduProjet/coinlang/branch/master)

# Récupération du dépôt Git
Ce dépôt dépends de sous-module(s). Pour pouvoir les récupérés en un coup avec le dépôt, la commande est :
```shell
git clone --recursive git@github.com:TristanEduProjet/CoinLang.git
```
Si le dépôt est déjà cloné, exécuter la commande suivante (si le dossier du module est vide) :
```shell
git submodule update --init --recursive
```

# Sujet

> Listes des fonctionnalités voulues :
> L’objectif du projet est de concevoir un interpréteur pour un mini langage.
> Il est obligatoire de baser l’interprétation sur un arbre de syntaxe abstrait construit au cours de l’analyse syntaxique du « programme » donné en entrée.  
> Le projet doit vous permettre d’acquérir les compétences suivantes :
>   * générer des analyseurs lexicaux et syntaxiques avec lex et yacc
>   * construire un arbre de syntaxe abstrait à partir d'un arbre concret
>   * interpréter la chaine d'entrée (le programme) en évaluant l'arbre de syntaxe abstrait correspondant.
> 
> Spécifications de la version minimale (11/20) :
>   1. Votre interpréteur devra gérer les noms de variables à plusieurs caractères.
>   2. Prendre en compte le type réel
>   3. Gérer les instructions suivantes :
>     a. affectation
>     b. affichage d’expressions numériques (pouvant contenir des variables numériques)
>     c. instructions conditionnelles : implémenter le `si-alors-sinon`/`si-alors`
>     d. structures itératives : implémenter le `while` et le `for`
>     e. Affichage de l’arbre de syntaxe et de la table des variables
> 
> Améliorations possibles (entre autre):
>   1. Gestion des erreurs (variable non initialisée, …)
>   2. Gestion de la saisie clavier
>   3. Gestion du type chaine de caractères (et extension d’autant de l’instruction d’affichage)
>   4. Gérer les fonctions
>   5. Gérer la déclaration explicite des variables
>   6. Gérer la portée des variables et les fonctions récursives
>   7. Les pointeurs
>   8. Les tableaux
>   9. la POO


# Fonctionnalités implémentées

* affectation des variables "simples" à un caractère
* reconnaissance des types :
    * nombres réels (flottant)
    * chaine de caractères
* calcul numérique de nombres
* remplacement de l'arbre stockant le programme généré par Yacc par une liste chainée modulaire 
* gestion des variables _(nom complet)_


# Fonctionnalités à venir

* reconnaissance de types supplémentaires :
    * boolean
    * nombres entiers
* expression booléennes
* instructions de base :
    * `if` et `else`
    * `if` ternaires
    * `for`
    * `while`
    * `switch`
* notion de "bloc" d'instruction(s) pour de futures fonctionnalités
* gestion basique de l'output
* gestion des fonctions
* vérification de la conformité des types (entre variables et fonctions)


# Fonctionnement interne

L’exécution du se sépare en 3 phases majeures :
  - Lecture et reconnaissance des paramètres
  - **Lecture du fichier source**
  - Affichage de la structure obtenue
  - **Vérification du programme obtenu**
  - **Exécution du programme**

## Lecture / Parser
Le fichier source est traité par Yacc qui fait appel à Lex pour lire le fichier. Ce dernier converti le fichier en *token* reconnaissable par Yacc qui vérifie et applique la grammaire du langage.

## Structure du programme généré
Le programme du script utilisateur est stockée sous une forme modulaire.  
Cela permet une gestion dynamique et plus souple du programme dans sa globalité.

Chaque instruction se calque sur une interface commune et définie comme elle doit être lue, évaluée, etc...  
Cela concerne (au niveau le plus abstrait) :
  - La vérification
  - L'affichage (_dump_)
  - L'exécution

_Cela permettra plus tard la mise en place d'optimisation plus facile._


# Utilisation
```
$> minicoin --help
Usage: minicoin [-h|--help] [--version] [-v|--verbose] [--silent] [--show-instru
cts={y,yes,n,no}] [--dump-session={y,yes,n,no}]  -f|-i|--file|--input|--in=<src>

Full command-line of minicoin.

  -h, --help                display this help and exit
      --version             display version info and exit
  -v, --verbose             verbose output
      --silent              silent output
      --show-instructs={y,yes,n,no}
                            show program obtain
      --dump-session={y,yes,n,no}
                            dump session after execution
                            (show variables and functions)
  -f, -i, --file, --input, --in=<src>
                            source file
```


# Dépendances
## Lex & Yacc
Le compilateur est construit à l'aide de [Flex](https://fr.wikipedia.org/wiki/Flex_(logiciel)) et [Bison](https://fr.wikipedia.org/wiki/GNU_Bison), forks récents de [Lex](https://fr.wikipedia.org/wiki/Lex_(logiciel)) et [Yacc](https://fr.wikipedia.org/wiki/Yacc_(logiciel)).

## Modules Git
Certaines dépendances de ce dépôt sont des modules Git (un lien vers des dépôts extérieures).  
Si le sous-modules est vide la première fois, il faut l'initialiser :
```shell
git submodule update --init
```
Si des mises à jour / commits viennent plus tard sur un sous-module, il faut faire dans l'ordre :
```shell
# mise à jour du dépôt parent
git fetch

# mise à jour du sous-module sur le bon commit
git submodule update
```

Pour plus de détail, consultez le wiki.

## [Argtable3](https://github.com/argtable/argtable3 "argtable/argtable3")
Librairie pour gérer les arguments du programme.  
En remplacement à **[Getopt](https://www.gnu.org/software/libc/manual/html_node/Getopt.html "GetOpt")** de GNU qui est plus long.

## [SGLIB](https://github.com/stefanct/sglib "/sglib")
_ **A Simple Generic Library for C** _
[Website](http://sglib.sourceforge.net/) [Documentation](http://sglib.sourceforge.net/doc/index.html#examples) [FreeCode](http://freecode.com/projects/sglib)

Librairie simple & générique de structure de base en C.  
Sa particularité est de n'utilisée que les macro (ou préprocesseur), et s'adapte donc au type contenu.

## [Klib](https://github.com/attractivechaos/klib "attractivechaos/klib")
Dans la même idée que SGLib, une librairie très légère reposant sur les macro pour de bonne performances et une adaptation aux types.  
Avec quelques extensions qui ne nous sont pas nécessaire ici.
