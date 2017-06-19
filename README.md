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
* calcul numérique de nombres


# Fonctionnalités à venir

* gestion des variables _(nom complet)_
* reconnaissance de types supplémentaires :
    * chaine de caractères
    * boolean
* expression booléennes
* notion de "bloc" d'instruction(s) pour de futures fonctionnalités
* ré-implémentation de l'arbre stockant le programme généré par Yacc
* gestion basique de l'output


# Dépendances
## Lex & Yacc
Le compilateur est construit à l'aide de [Flex](https://fr.wikipedia.org/wiki/Flex_(logiciel)) et [Bison](https://fr.wikipedia.org/wiki/GNU_Bison), forks récents de [Lex](https://fr.wikipedia.org/wiki/Lex_(logiciel)) et [Yacc](https://fr.wikipedia.org/wiki/Yacc_(logiciel)).s

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
