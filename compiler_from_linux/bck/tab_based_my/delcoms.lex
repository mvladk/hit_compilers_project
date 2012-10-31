/* Deleting comments */
%x comment1 comment2
%option noyywrap
%%
"/*"  BEGIN(comment1);
<comment1>[^*]*              /*do nothing till '*'  */
<comment1>\n                   /*delete new line in multi-line comment*/
<comment1>"*/"  BEGIN(0);
"//"   BEGIN(comment2);
<comment2>[^\n]                 /*do nothing till new line */
<comment2>\n   {ECHO;BEGIN(0);}
. | \n   ECHO;
%%

void main(int argc, char* argv[])
{
yyin=fopen(argv[1],"r");  /*input file name*/
yyout=fopen(argv[2],"w");   /*output file name*/
yylex();
}

