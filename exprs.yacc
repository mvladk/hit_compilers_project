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

%token start_prog start_com end_prog const id number exception integer real if then end_loop embed end_embed while raise ar_op rel_op

%%

PROGRAM:
	start_prog DEFINITIONS start_com COMMANDS end_prog
	|
	error_program COMMANDS
	;

DEFINITIONS:
	|
	DEFINITIONS
	|
	DEFINITION
	;
DEFINITION:
	TYPE  ID_LIST  
	///| 
	//TYPE const id := number 
	//| 
	//id : exception
	;

TYPE:
	integer  
	|
	real
	;

COMMANDS:
	COMMAND 
	| 
	COMMANDS
	;


COMMAND:
	id := EXPRESSION 
	|  
        if CONDITION then COMMANDS else COMMANDS end_loop  
	|  
	loop COMMANDS while CONDITION end_loop 
	|
	embed DEFINITIONS start_com COMMANDS end_embed
        raise id
	;

EXPRESSION:
	EXPRESSION ar_op EXPRESSION  
	| 
        number  
	|  
	id  
	| 
	(EXPRESSION)
	;

CONDITION:
	EXPRESSION  rel_op  EXPRESSION
	;
/*
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
*/
error_program:
	{
	  yyerror("Syntax error"); 
	}
	;
