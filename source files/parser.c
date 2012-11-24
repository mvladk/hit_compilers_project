#include "token.h"
#include "util.h"
#include <stdio.h>

int follow_DEFINITION(token *t);
int follow_DEFINITION_MAYBE(token *t);
int follow_DEFINITION_TYPE(token *t);
int follow_TYPE(token *t);
int follow_ID_LIST(token *t);
int follow_ID_LIST_MAYBE(token *t);
int follow_COMMAND_MAYBE(token *t);
int follow_COMMAND(token *t);
int follow_EXPRESSION_SIMPLE(token *t);
int follow_EXPRESSION_MAYBE(token *t);



token *t;

void parse_DEFINITIONS();
void parse_COMMANDS();
void parse_DEFINITION_TYPE();
void parse_DEFINITION_MAYBE();
void parse_DEFINITION();
void parse_CONDITION();
void parse_COMMAND_MAYBE();
void parse_COMMAND();
void parse_ID_LIST();


void parse_PROGRAM()
{
    puts("START_PROG DEFINITIONS START_COM COMMANDS END_PROG");
	match(START_PROG);
	parse_DEFINITIONS();
	match(START_COM);
	parse_COMMANDS();
	match(END_PROG);
	match(EOF);
}

void parse_DEFINITIONS()
{
	puts("DEFINITIONS -> DEFINITION DEFINITION_MAYBE");
	parse_DEFINITION();
	parse_DEFINITION_MAYBE();
}

void error_handler(int (*follow)(token *t), char *message) {
	printf("Error in line %d - %s\n", message);
    for (; !follow(t) && t->type != EOF; t = next_token()); 
	back_token();
}

void parse_DEFINITION_MAYBE()
{
	t = next_token();
	switch (t->type)
	{
	case SEMICOLON:
		puts("DEFINITION_MAYBE -> ; DEFINITIONS");
		parse_DEFINITIONS();
		break;
	case START_COM:
		puts("DEFINITION_MAYBE -> eps");
		back_token();
		break;
	default:
	    error_handler(follow_DEFINITION_MAYBE, "expected SEMICOLON");
		break;
	}
}

void parse_DEFINITION_TYPE()
{
	t = next_token();
	switch (t->type)
	{
	case ID:
		puts("DEFINITION_TYPE -> ID_LIST");
		back_token();
		parse_ID_LIST();
		break;
	case CONST:
		match(ID);
		match(ASSIGN);
		t = next_token();
		if (t->type != INT_LITERAL && t->type != REAL_LITERAL)
			error_handler(follow_DEFINITION_TYPE, "expected number");
		break;
	default:
		error_handler(follow_DEFINITION_TYPE, "expected const or id");
	}
}

void parse_DEFINITION()
{
	t = next_token();
	switch (t->type) {
	case ID:
		puts("DEFINITION -> id : exception");
		match(COLON);
		match(EXCEPTION);
		break;
	case INT:
	case REAL:
		puts("DEFINITION -> TYPE DEFINITION_TYPE");
		parse_DEFINITION_TYPE();
		break;
	default:
		error_handler(follow_DEFINITION, "expected int or real");
	}
}


void parse_TYPE()
{
	t = next_token();
	switch(t->type)
	{
	case INT:
		puts("TYPE -> integer");
		break;
	case REAL:
		puts("TYPE -> real");
		break;
	default:
		error_handler(follow_TYPE, "expected integer or real");
	}
}

void parse_ID_LIST_MAYBE() {
    t = next_token();
	switch (t->type) {
	case COMMA:
		puts("ID_LIST_MAYBE -> , id ID_LIST_MAYBE");
		match(ID);
		parse_ID_LIST_MAYBE();
		break;
	case SEMICOLON:
	case START_COM:
		puts("ID_LIST_MAYBE -> eps");
		back_token();
		break;
	default:
		error_handler(follow_ID_LIST_MAYBE, "expected comma");
	}
}

void parse_ID_LIST() {
    t = next_token();
	switch (t->type)
	{
	case ID:
		puts("ID_LIST -> id ID_LIST_MAYBE");
		parse_ID_LIST_MAYBE();
		break;
	default:
		error_handler(follow_ID_LIST, "expected id");
	}
}

void parse_COMMANDS() {
	puts("COMMANDS -> COMMAND COMMAND_MAYBE");
	parse_COMMAND();
	parse_COMMAND_MAYBE();
}

void parse_COMMAND_MAYBE() {
	t = next_token();
	switch (t->type) 
	{
	case SEMICOLON:
		puts("COMMAND_MAYBE -> ; COMMANDS");
		parse_COMMANDS();
		break;
	case END_PROG:
	case ELSE:
	case END_IF:
	case WHILE:
	case END_EMBED:
		puts("COMMAND_MAYBE -> eps");
		back_token();
		break;
	default:
		error_handler(follow_COMMAND_MAYBE, "expected SEMICOLON");
	}
}

void parse_EXPRESSION();

void parse_COMMAND() {
	t = next_token();
	switch (t->type) {
	case ID:
		puts("COMMAND -> id := EXPRESSION");
		match(ASSIGN);
		parse_EXPRESSION();
		break;
	case LOOP:
		parse_COMMANDS();
		match(WHILE);
		parse_CONDITION();
		match(END_LOOP);
		break;
	case EMBED:
		parse_DEFINITIONS();
		match(START_COM);
		parse_COMMANDS();
		match(END_EMBED);
		break;
	case RAISE:
		match(ID);
		break;
	default:
		error_handler(follow_COMMAND, "expected id or if or LOOP or embed or RAISE");
	}
}

void parse_EXPRESSION_SIMPLE()
{
	t = next_token();
	switch (t->type)
	{
	case INT_LITERAL:
		puts("EXPRESSION_SIMPLE -> NUMBER");
		break;
	case REAL_LITERAL:
		puts("EXPRESSION_SIMPLE -> NUMBER");
		break;
	case ID:
		puts("EXPRESSION_SIMPLE -> id");
		break;
	case LEFT_BRACKET:
		puts("EXPERSSION_SIMPLE -> (EXPRESSION)");
		parse_EXPRESSION();
		match(RIGHT_BRACKET);
		break;
	default:
		error_handler(follow_EXPRESSION_SIMPLE, "expected number or id or (");
	}
}

void parse_EXPRESSION_MAYBE();

void parse_EXPRESSION() {
	puts("EXPRESSION -> EXPRESSION_SIMPLE EXPRESSION_MAYBE");
	parse_EXPRESSION_SIMPLE();
	parse_EXPRESSION_MAYBE();
}

void parse_EXPRESSION_MAYBE() 
{
	t = next_token();
	switch (t->type)
	{
	case AR_OP:
		puts("EXPRESSION_MAYBE -> AR_OP EXPRESSION");
		parse_EXPRESSION();
		break;
	case(RIGHT_BRACKET):
	case SEMICOLON:
	case END_PROG:
	case ELSE:
	case END_IF:
	case WHILE:
	case END_EMBED:
	case REL_OP:
	case END_LOOP:
	case THEN:
		puts("EXPRESSION_MAYBE -> eps");
		break;
	default:
		error_handler(follow_EXPRESSION_MAYBE, "expected AR_OP");
	}
}

void parse_CONDITION()
{
	puts("CONDITION -> EXPRESSION REL_OP EXPRESSION");
	parse_EXPRESSION();
	match(REL_OP);
	parse_EXPRESSION();
}