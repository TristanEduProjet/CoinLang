%option noyywrap
%{

#include <stdlib.h>
#include <string.h>
#include "minicoin.parser.h"

%}

empty [ \t\n]+

num   [0-9]
int   {num}+
exp   [eE][+-]?{int}
var   [a-zA-Z_][a-zA-Z0-9_]+
real  {int}("."{int})?{exp}?

%%

{empty} { /* Nothing */ }


"("   return(OP_PAR);
")"   return(CL_PAR);

{var} {
    yylval.str = (char*)strdup(yytext);
    return(VAR);
}

{real} {
    yylval.real = atof(yytext);
    return(NUM);
}

"+"   return(PLUS);
"-"   return(MIN);
"*"   return(MULT);
"/"   return(DIV);
"^"   return(POW);

"="   return(AFF);

";"   return(COLON);

<<EOF>>  return(END);
