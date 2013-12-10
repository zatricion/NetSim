
%{
#include <iostream>
/* Include tokens and variables from the parser (NUM, OPEN, CLOSE, yylval) */
#include "parser.tab.hpp"
%}
%option noyywrap

%%

host         return HOSTTOK;
router       return ROUTERTOK;
link         return LINKTOK;
"["          return LBRACK;
"]"          return RBRACK;
vegas        return VEGASFLOWTOK;
tahoe        return TAHOEFLOWTOK;

[A-Za-z]+[0-9]* {yylval.strval = yytext;
                 return STR;}

[-+]?(([0-9]*\.?[0-9]*)|(\.[0-9]+))([Ee][+-]?[0-9]+)? {
                       yylval.fval = atof(yytext); 
		       return FLOAT;}

#.*$         /* Eat comments   */
[ \t\r\n]+   /* Eat whitespace */

%%
