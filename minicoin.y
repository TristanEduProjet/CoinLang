%{
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
    int num;
    bool bln;
    Instr *instr;
}

%token  <real> NUM_REAL
%token  <num>  NUM_INT
%token  <str>  VAR STR
%token  <bln>  BOOL
%token  PLUS MIN MULT DIV POW
%token  OP_PAR CL_PAR OP_ACL CL_ACL AFF
%token  NOT AND OR XOR
%token  COLON END

%type  <instr> Instlist Inst
%type  <instr> Expr Expr_Numeric Expr_Boolean

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
  | OP_ACL Instlist CL_ACL { $$ = $2; }
  ;

Expr:
    Expr_Numeric
  | Expr_Boolean
  | STR { $$ = (Instr*) newInstrExpr(DT_STRING, $1); }
  | OP_PAR Expr CL_PAR { $$ = $2; }
  ;

Expr_Numeric:
    NUM_REAL  { $$ = (Instr*) newInstrExpr(DT_REAL, $1); }
  | NUM_INT   { $$ = (Instr*) newInstrExpr(DT_INT, $1); }
  | Expr PLUS Expr     { $$ = (Instr*) newInstrCalc(OP_PLUS, $1, $3); }
  | Expr MIN Expr      { $$ = (Instr*) newInstrCalc(OP_MIN, $1, $3); }
  | Expr MULT Expr     { $$ = (Instr*) newInstrCalc(OP_MULT, $1, $3); }
  | Expr DIV Expr      { $$ = (Instr*) newInstrCalc(OP_DIV, $1, $3); }
  | MIN Expr %prec NEG { $$ = (Instr*) newInstrCalc(OP_MIN, NULL, $2); /* ? */ }
  | Expr POW Expr      { $$ = (Instr*) newInstrCalc(OP_POW, $1, $3); }
  ;

Expr_Boolean:
    BOOL  { $$ = (Instr*) newInstrExpr(DT_BOOL, $1); }
  | NOT Expr_Boolean              { $$ = (Instr*) newInstrLogic(LT_NOT, $2, NULL); }
  | Expr_Boolean AND Expr_Boolean { $$ = (Instr*) newInstrLogic(LT_AND, $1, $3); }
  | Expr_Boolean OR Expr_Boolean  { $$ = (Instr*) newInstrLogic(LT_OR, $1, $3); }
  | Expr_Boolean XOR Expr_Boolean { $$ = (Instr*) newInstrLogic(LT_XOR, $1, $3); }
  ;

%%

static inline void yyerror(const Instr **l, const char *s) {
    fprintf(stderr, "%s\n", s);
}
