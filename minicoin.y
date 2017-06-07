%{
#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>
#include <string.h>
#include "minicoin_tree.h"
#include "minicoin_eval.h"
#include "unorderedmap.h"

extern int yyparse();
extern FILE *yyin;

static Node root;
extern const HashMap *variables;

void exec(const Node *node);
void yyerror(const char *s);
%}

%union {
    struct Node *node;
}

%token  <node> NUM VAR
%token  <node> PLUS MIN MULT DIV POW
%token  OP_PAR CL_PAR COLON AFF
%token  EOL

%type  <node> Instlist
%type  <node> Inst
%type  <node> Expr

%left  OR AND
%left  EQ NEQ
%left  GT LT GET LET
%left  PLUS MIN
%left  MULT DIV
%left  NEG NOT
%right POW

%start Input
%%

Input:
    /* Nothing ... */
  | Line Input { /* Nothing ... */ }

Line:
    EOL {  }
  | Instlist EOL { exec($1); }
  ;

Instlist:
    Inst { $$ = nodeChildren(createNode(NTINSTLIST), $1, createNode(NTEMPTY)); }
  | Instlist Inst { $$ = nodeChildren(createNode(NTINSTLIST), $1, $2); }
  ;

Inst:
    Expr COLON { $$ = $1; }
  | VAR AFF Expr COLON {printf("variable : %s\n", $1->var); $$ = $3;}
  ;

Expr:
    NUM     { $$ = $1; }
  | Expr PLUS Expr     { $$ = nodeChildren($2, $1, $3); }
  | Expr MIN Expr      { $$ = nodeChildren($2, $1, $3); }
  | Expr MULT Expr     { $$ = nodeChildren($2, $1, $3); }
  | Expr DIV Expr      { $$ = nodeChildren($2, $1, $3); }
  | MIN Expr %prec NEG { $$ = nodeChildren($1, createNode(NTEMPTY), $2); }
  | Expr POW Expr      { $$ = nodeChildren($2, $1, $3); }
  | OP_PAR Expr CL_PAR { $$ = $2; }
  ;

%%

void exec(const Node *node) {
    printGraph(node);
    eval(node);
}

void yyerror(const char *s) {
    printf("%s\n", s);
}

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
