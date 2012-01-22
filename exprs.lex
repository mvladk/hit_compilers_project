%{
   #include <iostream>
    #include <string>
   #define YYSTYPE std::string
   #include "y.exprs.h"
   void yyerror(char *s);
%}

%option yylineno
%option noyywrap

%x STR
DIGIT [0-9]
%%

[/][/].*\n      ; // comment

start_prog 	return START_PROG;
end_prog 	return END_PROG;
start_com 	return START_COM;
exception	return EXCEPTION;
const		return CONST;
:=		{yylval = yytext;return ASSIGN;}
integer		return INT;
real		return REAL;


if              return IF;
then		return THEN;
else            return ELSE;
while           return WHILE;
end_loop	return END_LOOP;
loop		return LOOP;
embed		return EMBED;
end_embed	return END_EMBED;
raise		return RAISE;
 /*
exit            return EXIT;
*/
==              { yylval = yytext;return rel_op_1;}
[<]=            { yylval = yytext;return rel_op_1;}
>=              { yylval = yytext;return rel_op_1;}
[>]{1}          { yylval = yytext;return rel_op_1;}
[<]{1}          { yylval = yytext;return rel_op_1;}
!=              { yylval = yytext;return rel_op_1;}


[-+/] 		{ yylval = yytext;
                  return ar_op_1;
                }
[*]{1,2}	{ yylval = yytext;
                  return ar_op_1;
                }
0"."{DIGIT}+|[1-9]{DIGIT}*"."{DIGIT}+    {
		  yylval = yytext;
                  return NUM_1;}

0|[1-9]{DIGIT}*          { yylval = yytext;
                  return NUM_1;
                }

[a-zA-Z]+([a-zA-Z0-9_]*[a-zA-Z0-9]+)* { yylval = yytext;
                  return ID_1;
                }
["]             { yylval = ""; BEGIN(STR); }
<STR>[^\\\n"]+  yylval += yytext;
<STR>\\n        yylval += '\n';
<STR>\\["]      yylval += '"';
<STR>\\         yyerror("Invalid escape sequence");
<STR>\n         yyerror("Newline in string literal");
<STR>["]        { BEGIN(INITIAL); return STRING; }
[ \t\r\n]       ; // whitespace

[{};:()<>!,] { return *yytext; }
.               yyerror("Invalid character");

%%
