%option noyywrap
%option noinput
%option nounput
%{
#include <stdio.h>
#include "y.tab.h"
FILE* yyin;
%}

%%

[0-9]+ { yylval.val = atoi(yytext); return INT; }
"+"    { return ADD; }
"*"    { return MUL; }
"\n"   { return 0; }
.      { /* ignore everything else */ }

%%
