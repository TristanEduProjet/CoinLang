%option noyywrap
%{

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "minicoin.parser.h"

char buffer[SHRT_MAX];
char *ptr_buf;
%}
empty [ \t\n]+

num   [0-9]
int   {num}+
exp   [eE][+-]?{int}
var   [a-zA-Z_][a-zA-Z0-9_]+
real  ({int}?"."{int}{exp}?|{int}({exp}{int}|[lL]?[fF]))

%x String

%%

{empty} { /* Nothing */ }


"("   return(OP_PAR);
")"   return(CL_PAR);

"{"   return(OP_ACL);
"}"   return(CL_ACL);

\"      { BEGIN(String);
          ptr_buf = buffer; }
<String>\\n  *ptr_buf++ = '\n';
<String>\\t  *ptr_buf++ = '\t';
<String>\\r  *ptr_buf++ = '\r';
<String>\\b  *ptr_buf++ = '\b';
<String>\\f  *ptr_buf++ = '\f';
<String>\\\" *ptr_buf++ = '\"';
<String>\\[0-2]{0,1}[0-7]{2,3} { //[0-7]{1,3} /* octal escape sequence */
                int result;
                sscanf( yytext + 1, "%o", &result );
                if( result > 0xff ) {
                    fprintf(stderr, "Octal constant is out-of-bounds : %s\n", yytext);
                    exit(EXIT_FAILURE);
                } else
                    *ptr_buf++ = result;
        }
<String>\\[0-9]+ { fprintf(stderr, "bad escape sequence : %s\n", yytext);
                   exit(EXIT_FAILURE); }
<String>\"  { *ptr_buf = '\0';
                BEGIN(INITIAL); //BEGIN 0;
                yylval.str = (char*)strdup(buffer);
                return(STR);
            }
<String>\n  { fputs("unterminated string constant\n", stderr); //invalid string
                exit(EXIT_FAILURE); }
<String>.   { *ptr_buf++ = *yytext; /*}

<str>\\(.|\n)  *ptr_buf++ = yytext[1];
<str>[^\\\n\"]+  {
        char *yptr = yytext;
        while( *yptr )
            *ptr_buf++ = *yptr++;*/
        }

{real} {
    yylval.real = atof(yytext);
    return(NUM_REAL);
}

{int} {
    yylval.num = atoi(yytext);
    return(NUM_INT);
}

"+"   return(PLUS);
"-"   return(MIN);
"*"   return(MULT);
"/"   return(DIV);
"^"   return(POW);

"true" {
    yylval.bln = true;
    return(BOOL);
}
"false" {
    yylval.bln = false;
    return(BOOL);
}
"not"  return(NOT);
"and"  return(AND);
"or"   return(OR);
"xor"  return(XOR);

">"    return(GT);
">="   return(GET);
"<"    return(LT);
"<="   return(LET);
"=="   return(EQ);
"!="   return(NEQ);

"if"   return(IF);
"else" return(ELSE);
"?"    return(TERN_THEN);
":"    return(TERN_ELSE);

"="    return(AFF);

";"    return(COLON);

{var} {
    yylval.str = (char*)strdup(yytext);
    return(VAR);
}
