%{
#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>
#include <string.h>
#include "minicoin_inst.h"

static inline void yyerror(const Instr **r, const char *s);
%}

%code requires {
#include "minicoin_inst.h"
}

%union {
    char *str;
    double real;
    Instr *instr;
}

%token  <real> NUM
%token  <str> VAR
%token  PLUS MIN MULT DIV POW
%token  OP_PAR CL_PAR AFF
%token  COLON END

%type  <instr> Instlist
%type  <instr> Inst
%type  <instr> Expr

%left  OR AND
%left  EQ NEQ
%left  GT LT GET LET
%left  PLUS MIN
%left  MULT DIV
%left  NEG NOT
%right POW

%parse-param {const Instr **root}
%start Input
%%

Input:
    /* Nothing ... */
  | Line Input { /* Nothing ... */ }

Line:
    END { YYACCEPT; }
  | Instlist END { *root = $1; }
  ;

Instlist:
    Inst { $$ = (Instr*) newInstrList(); addInstrList((InstrList*)$$, $1); }
  | Instlist Inst { $$ = $1; addInstrList((InstrList*)$$, $2); }
  ;

Inst:
    Expr COLON { $$ = $1; }
  | VAR AFF Expr COLON {$$ = (Instr*) newInstrAffect($1, $3);}
  ;

Expr:
    NUM     { $$ = (Instr*) newInstrExpr(DT_REAL, &$1); }
  | Expr PLUS Expr     { $$ = (Instr*) newInstrCalc(OP_PLUS, $1, $3); }
  | Expr MIN Expr      { $$ = (Instr*) newInstrCalc(OP_MIN, $1, $3); }
  | Expr MULT Expr     { $$ = (Instr*) newInstrCalc(OP_MULT, $1, $3); }
  | Expr DIV Expr      { $$ = (Instr*) newInstrCalc(OP_DIV, $1, $3); }
  | MIN Expr %prec NEG { $$ = (Instr*) newInstrCalc(OP_MIN, NULL, $2); /* ? */ }
  | Expr POW Expr      { $$ = (Instr*) newInstrCalc(OP_POW, $1, $3); }
  | OP_PAR Expr CL_PAR { $$ = $2; }
  ;

%%

static inline void yyerror(const Instr **l, const char *s) {
    fprintf(stderr, "%s\n", s);
}
