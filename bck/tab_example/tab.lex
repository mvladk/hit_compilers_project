%{
#include <stdio.h>
#include "y.tab.h"
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
