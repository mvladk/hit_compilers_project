%{
   #include "token.h"
   void yyerror(char *s) {}
   int line_number = 1;
%}

%option noyywrap
%option never-interactive

%x STR
DIGIT [0-9]
%%

[/][/].*"\n"      line_number++; // comment
"\n"              line_number++;
START_PROG 	return START_PROG;
END_PROG 	return END_PROG;
START_COM 	return START_COM;
exception	return EXCEPTION;
const		return CONST;
:=		{return ASSIGN;}
integer		return INT;
real		return REAL;


if              return IF;
THEN		return THEN;
else            return ELSE;
while           return WHILE;
END_LOOP	return END_LOOP;
LOOP		return LOOP;
embed		return EMBED;
END_EMBED	return END_EMBED;
RAISE		return RAISE;
 /*
exit            return EXIT;
*/
==              { return REL_OP;}
[<]=            { return REL_OP;}
>=              { return REL_OP;}
[>]{1}          { return REL_OP;}
[<]{1}          { return REL_OP;}
!=              { return REL_OP;}


[-+/] 		{     return AR_OP;
                }
[*]{1,2}	{     return AR_OP;
                }
0"."{DIGIT}+|[1-9]{DIGIT}*"."{DIGIT}+    {
		          return REAL_LITERAL;}

0|[1-9]{DIGIT}*          { 
                  return INT_LITERAL;
                }

[a-zA-Z]+([_].[a-zA-Z0-9]+)*[a-zA-Z0-9]* { 
                  return ID;
                }
[ \t\r\n]       ; // whitespace

[{};:()<>!,] { return *yytext; }
.|__            {  printf("BadChar: %s ", yytext); yyerror("Invalid character"); }

%%
