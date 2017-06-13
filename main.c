#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iso646.h>
#include "extern.h"
#include "minicoin_eval.h"

extern int yyparse();
extern FILE *yyin;

int main(const int argc, const char *argv[]) {
    if(variables == NULL)
        fprintf(stderr, "Erreur initialisation interne\n");
    else
        if ((argc == 3) and (strcmp(argv[1], "-f") == 0)) {
            /*const*/ FILE *fp = fopen(argv[2], "r");
            if(fp == NULL) {
                fprintf(stderr, "Impossible d'ouvrir le fichier � executer.\n");
            } else {
                Node *root = NULL;
                yyin = fp;
                printf(">< Parsing :\n");
                const int res = yyparse(&root);
                fclose(fp);
                if(res == 0) {
                    printf("\n>< Instructions :\n");
                    printGraph(root);
                    printf("\n>< Execution du programme :\n");
                    eval(root);
                    return EXIT_SUCCESS;
                } else {
                    fprintf(stderr, "yyparse() return code %d\n", res);
                    return res;
                }
            }
        } else {
            fprintf(stderr, "Arguments invalides.\n");
        }
    return EXIT_FAILURE;
}
