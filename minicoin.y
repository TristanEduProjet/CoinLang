%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "minicoin_tree.h"
#include "minicoin_eval.h"

extern int  yyparse();
extern FILE *yyin;


Node root;
 

%}

%union {
	struct Node *node;
}


%token   <node> NUM VAR
%token   <node> PLUS MIN MULT DIV POW 
%token   OP_PAR CL_PAR COLON AFF
%token   EOL


%type   <node> Instlist
%type   <node> Inst
%type   <node> Expr
  

%left OR
%left AND
%left EQ NEQ
%left GT LT GET LET
%left PLUS  MIN
%left MULT  DIV
%left NEG NOT
%right  POW

%start Input
%%

Input:
      {/* Nothing ... */ }
  | Line Input { /* Nothing ... */ }


Line:
    EOL {  }
  | Instlist EOL { exec($1);    }
  ; 

Instlist:
    Inst { $$ = nodeChildren(createNode(NTINSTLIST), $1, createNode(NTEMPTY)); } 
  | Instlist Inst { $$ = nodeChildren(createNode(NTINSTLIST), $1, $2); }
  ;

Inst:
    Expr COLON { $$ = $1; } 
	|VAR AFF Expr COLON {printf("variable :%s\n",$1->var);$$ = $3;}
  ;


Expr:
  NUM			{ $$ = $1; }
    | Expr PLUS Expr     { $$ = nodeChildren($2, $1, $3); }
  | Expr MIN Expr      { $$ = nodeChildren($2, $1, $3); }
  | Expr MULT Expr     { $$ = nodeChildren($2, $1, $3); }
  | Expr DIV Expr      { $$ = nodeChildren($2, $1, $3); }
  | MIN Expr %prec NEG { $$ = nodeChildren($1, createNode(NTEMPTY), $2); }
  | Expr POW Expr      { $$ = nodeChildren($2, $1, $3); }
  | OP_PAR Expr CL_PAR { $$ = $2; }
  ;


%%

 
 

int exec(Node *node) {
   printGraph(node);
  eval(node);
}

 

int yyerror(char *s) {
  printf("%s\n", s);
}

 

int main(int arc, char **argv) {
   if ((arc == 3) && (strcmp(argv[1], "-f") == 0)) {
    
    FILE *fp=fopen(argv[2],"r");
    if(!fp) {
      printf("Impossible d'ouvrir le fichier à executer.\n");
      exit(0);
    }      
    yyin=fp;
    yyparse();
		  
    fclose(fp);
  }  
  exit(0);
}
