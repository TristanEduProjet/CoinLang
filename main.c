#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iso646.h>
#include "extern.h"

extern int yyparse();
extern FILE *yyin;

int main(const int argc, const char *argv[]) {
    if(variables == NULL)
        fprintf(stderr, "Erreur initialisation interne\n");
    else
        if ((argc == 3) and (strcmp(argv[1], "-f") == 0)) {
            /*const*/ FILE *fp = fopen(argv[2], "r");
            if(fp == NULL) {
                fprintf(stderr, "Impossible d'ouvrir le fichier à executer.\n");
            } else {
                yyin = fp;
                yyparse();
                fclose(fp);
                return EXIT_SUCCESS;
            }
        } else {
            fprintf(stderr, "Arguments invalides.\n");
        }
    return EXIT_FAILURE;
}
