/**
 * c/c++
 *
 * Art Brindatch(tm) : mvcompiler (http://art-brindatch.com)
 * Copyright 2005-2012, Art Brindatch(Vladimir Karpov), Ltd. (http://art-brindatch.com)
 *
 * Licensed under The MIT License
 * Redistributions of files must retain the above copyright notice.
 *
 * @copyright     Copyright 2005-2013, Art Brindatch(Vladimir Karpov), Ltd. (http://art-brindatch.com)
 * @link          http://art-brindatch.com mvcompiler(tm) Project
 * @since         mvcompiler (tm) v 1.0.0
 * @license       MIT License (http://www.opensource.org/licenses/mit-license.php)
 */
%{
   #include "token.h"
   void yyerror(char *s) {}
   int line_number = 1;
   FILE *log_file;
%}

%option noyywrap
%option never-interactive

%x STR
DIGIT [0-9]
%%

[/][/].*"\n"      line_number++; // comment
"\n"              line_number++;
start_prog 	return START_PROG;
end_prog 	return END_PROG;
start_com 	return START_COM;
exception	return EXCEPTION;
const		return CONST;
:=		{return ASSIGN;}
integer		return INT;
real		return REAL;
;           return SEMICOLON;
,           return COMMA;
:           return COLON;
"("         return LEFT_BRACKET;
")"         return RIGHT_BRACKET;
if              return IF;
then		return THEN;
else            return ELSE;
while           return WHILE;
end_loop	return END_LOOP;
end_if      return END_IF;
loop		return LOOP;
embed		return EMBED;
end_embed	return END_EMBED;
raise		return RAISE;
 /*
exit            return EXIT;
*/
==              { return REL_OP;}
[<]=            { return REL_OP;}
>=              { return REL_OP;}
[>]{1}          { return REL_OP;}
[<]{1}          { return REL_OP;}
!=              { return REL_OP;}


[\-+/] 		{     return AR_OP;
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
.|__            {  printf("At Line %d - Bad Char %s\n", line_number, yytext);
                   fprintf(log_file, "At Line %d - Bad Char %s\n", line_number, yytext);
                   yyerror("Invalid character"); }
<<EOF>>      return EOF;
%%
int main() {
    log_file = fopen("log.txt", "w");
    parse_PROGRAM();
    fclose(log_file);
    return 0;
}
