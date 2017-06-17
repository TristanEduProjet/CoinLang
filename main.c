#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iso646.h>
#include "minicoin_eval.h"
#include "unorderedmap.h"

extern int yyparse();
extern FILE *yyin;

typedef struct Param {
    char *file_input;
} Param;

inline Param parse_args(const int argc, const char *argv[]);
inline Node* parse_yy(const Param params);
inline void exec_prog(const Node *root);

int main(const int argc, const char *argv[]) {
    const Param params = parse_args(argc, argv);
    const Node *root = parse_yy(params);
    if(root != NULL) {
        printf("\n>< Instructions :\n");
        printGraph(root);
        exec_prog(root);
        return EXIT_SUCCESS;
    } else
        return EXIT_FAILURE;
}

inline Param parse_args(const int argc, const char *argv[]) {
    Param params;
    if ((argc == 3) and (strcmp(argv[1], "-f") == 0)) {
            params.file_input = argv[2];
    } else {
        fprintf(stderr, "Arguments invalides.\n");
        exit(EXIT_FAILURE);
    }
    return params;
}

inline Node* parse_yy(const Param params) {
    printf("\n>- Preparation du parser :\n");
    /*const*/ FILE *fp = fopen(params.file_input, "r");
    if(fp == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier à executer.\n");
        exit(EXIT_FAILURE);
    } else {
        yyin = fp;
        Node *root = NULL;
        printf("\n>< Parsing :\n");
        const int res = yyparse(&root);
        fclose(fp);
        if(res == 0)
            return root;
        else {
            fprintf(stderr, "yyparse() return code %d\n", res);
            exit(res);
        }
    }
}

inline void exec_prog(const Node *root) {
    printf("\n>- Preparation du programme :\n");
    const HashMap *variables = HashMap_new();
    if(variables == NULL) {
        fprintf(stderr, "Erreur initialisation interne\n");
        exit(EXIT_FAILURE);
    } else {
        printf("\n>< Execution du programme :\n");
        eval(root);
        HashMap_free(variables);
    }
}
