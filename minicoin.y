%{
#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>
#include <string.h>
#include "minicoin_tree.h"
#include "minicoin_eval.h"
#include "unorderedmap.h"
#include "extern.h"

static Node root;

inline void exec(const Node *node);
inline void yyerror(const char *s);
%}

%union {
    struct Node *node;
}

%token  <node> NUM VAR
%token  <node> PLUS MIN MULT DIV POW
%token  OP_PAR CL_PAR AFF
%token  COLON EOL

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

inline void exec(const Node *node) {
    printGraph(node);
    eval(node);
}

inline void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
}
