#include <stdio.h>
#include "token.h"

char *token_name(int token_type)
{
	switch (token_type)
	{
        case WHILE: return "WHILE" ;
        case START_COM: return "START_COM" ;
        case RAISE: return "RAISE" ;
        case LOOP: return "LOOP" ;
        case ID: return "ID" ;
        case END_EMBED: return "END_EMBED" ;
        case EMBED: return "EMBED" ;
        case END_LOOP: return "END_LOOP" ;
        case ASSIGN: return "ASSIGN" ;
        case SEMICOLON: return "SEMICOLON" ;
        case THEN: return "THEN" ;
        case START_PROG: return "START_PROG" ;
        case RIGHT_BRACKET: return "RIGHT_BRACKET" ;
        case REL_OP: return "REL_OP" ;
        case REAL_LITERAL: return "REAL_LITERAL" ;
        case REAL: return "REAL" ;
        case INT: return "INT" ;
        case INT_LITERAL: return "INT_LITERAL" ;
        case END_PROG: return "END_PROG" ;
        case LEFT_BRACKET: return "LEFT_BRACKET" ;
        case EXCEPTION: return "EXCEPTION" ;
        case COLON: return "COLON" ;
        case COMMA: return "COMMA" ;
        case END_IF: return "END_IF" ;
        case ELSE: return "ELSE" ;
        case AR_OP: return "AR_OP" ;
        case IF: return "IF" ;
        case CONST: return "CONST" ;
        case EOF: return "EOF";
	}
}

void match(int token_type)
{
    extern FILE *log_file;
	token t = next_token();
	if (t.type != token_type) {
		printf("At line %d - expected %s, got %s\n", t.line, token_name(token_type), token_name(t.type));
        fprintf(log_file, "At line %d - expected %s, got %s\n", t.line, token_name(token_type), token_name(t.type));      
		return;
	}
}
