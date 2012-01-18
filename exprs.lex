%{
#include <stdio.h>
#include "y.exprs.h"
%}
%%
[0-9]+                  return NUMBER;
heat                    return TOKHEAT;
on|off                  return STATE;
target                  return TOKTARGET;
temperature             return TOKTEMPERATURE;
start_prog             return start_prog;
\n                      /* ignore end of line */;
[ \t]+                  /* ignore whitespace */;
%%
void main(int argc, char* argv[])
{
yyin=fopen(argv[1],"r");
if(argv[2])  yyout=fopen(argv[2],"w");
else yyout=stdout;
//yylex();
yyparse();
}
