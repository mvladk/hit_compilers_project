%{
int words, chars;
%}
%option noyywrap
DIGIT [0-9]
ID       [a-z][a-z0-9]*
%%
0|[1-9]{DIGIT}*	      {fprintf(yyout,"\nInteger:%s\n",yytext);}
0"."{DIGIT}+|[1-9]{DIGIT}*"."{DIGIT}+    {fprintf(yyout,"\nReal:%s",yytext);}
"+"|"/"|":="|"="|"-"|"*"|";"   {fprintf(yyout,"\nAn operator:%s",yytext);}
start_prog             return start_prog;

start_com|end_prog|const|exception        {
                       printf( "A keyword: %s\n", yytext );
                       }
if|then|else|loop|end_loop|embed|end_embed|raise|while|function 		{
                       printf( "A keyword: %s\n", yytext );
                       }
[a-zA-Z]+       {words++;chars += yyleng; fprintf(yyout,"\nVar name:%s\n",yytext);}
"{"[^}\n]*"}" ;    /* eat up one-line comments */

[ \t\n]+  ;        /* eat up whitespace */

.                             fprintf(yyout,"\nUnrecognized character: %s",yytext);
%%
void main(int argc, char* argv[])
{
yyin=fopen(argv[1],"r");
if(argv[2])  yyout=fopen(argv[2],"w");
else yyout=stdout;
yylex();
}

