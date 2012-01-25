%{

    #include <iostream>
    #include <string>
    #include <sstream>
    extern int yylineno;
    extern int yylex();
    extern FILE * yyin;
    extern FILE * yyout;
    const int MAXAR = 1000;
    struct one_token {
      char * token_type;
      std::string lexeme;
      int line;
    }  onetoken;
      /*
    class token_list
    {
	one_token token_array[MAXAR];
        int i=0;
	public:
	void token_print(char *s,std::string l) {      
	  this->token_array[i].token_type=s;
	  this->i++;
	  std::cerr << "token type: "<< tok1.token_type << ", lexeme: " << l << ", line " << yylineno << std::endl;

	}
    }
    */
   
    void yyerror(char *s) {
      std::cerr << s << ", line " << yylineno << std::endl;
     }
    void token_print(char *s,std::string l) {   
      std::cerr << "token type: "<< s << ", lexeme: " << l << ", line " << yylineno << std::endl;
     }

    #define YYSTYPE std::string

%}

%token START_PROG END_PROG START_COM INT REAL CONST ID_1 ASSIGN NUM_1 STRING EXCEPTION ar_op_1 rel_op_1
%token IF THEN ELSE WHILE END_LOOP LOOP EMBED END_EMBED RAISE


%%
PROGRAM:
	START_PROG DEFINITIONS START_COM COMMANDS END_PROG
	;
DEFINITIONS:
	|
	DEFINITION ';'
	|
	DEFINITION ';' DEFINITIONS
	;
DEFINITION:
	TYPE ID_LIST
	|
	TYPE CONST ID assignment NUM 
	|
	ID ':' EXCEPTION
	;

ID_LIST:
	ID
	|
	ID ',' ID_LIST
	;
TYPE:
	INT  
	|
	REAL
	;

COMMANDS:
	COMMAND
	| 
	COMMAND ';' COMMANDS
	;


COMMAND:
	ID assignment EXPRESSION
	|
	IF CONDITION THEN COMMANDS ELSE COMMANDS END_LOOP
	|  
	LOOP COMMANDS WHILE CONDITION END_LOOP
	|
	EMBED DEFINITIONS START_COM COMMANDS END_EMBED
	|
	RAISE ID
	;

EXPRESSION:
	EXPRESSION ar_op EXPRESSION 
	|
	ID
	|
	NUM
	|
	'(' EXPRESSION ')'
	;

CONDITION:
	EXPRESSION  rel_op  EXPRESSION
	;
ID:
	ID_1
	{
	  token_print("id    ",$$);
	}
	;
NUM:    
	NUM_1
	{
	  token_print("number",$$);
	}
assignment :
	ASSIGN
	{
	   token_print("assign",$$);
	}
	;
ar_op:
	ar_op_1
	{
	   token_print("ar_op ",$$);
	}
	;
rel_op:
	rel_op_1
	{
	   token_print("rel_op",$$);
	}
	;
%%

int main(int argc, char *argv[]) {
    yyin=fopen(argv[1],"r");
    if(argv[2])  yyout=fopen(argv[2],"w");
    else yyout=stdout;

  return yyparse();
}

