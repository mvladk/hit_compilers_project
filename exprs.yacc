%{
#include <stdio.h>
#include <string.h>

void yyerror(const char *str)
{
	fprintf(stderr,"error: %s\n",str);
}

int yywrap()
{
	return 1;
}

extern FILE *yyout;

/*main()
{
yyparse();
}*/

%}

%token NUMBER TOKHEAT STATE TOKTARGET TOKTEMPERATURE start_prog

%%

program:
	start_prog commands
	|
	error_program commands
	;

commands: /* empty */
	| commands command
	;


command:
	heat_switch
	|
	target_set
	;

heat_switch:
	TOKHEAT STATE 
	{
		printf("\tHeat turned on or off\n");
	}
	;

target_set:
	TOKTARGET TOKTEMPERATURE NUMBER
	{
		printf("\tTemperature set XXX\n");
	}
	;
error_program:
	{
	  yyerror("Syntax error"); 
	}
	;
