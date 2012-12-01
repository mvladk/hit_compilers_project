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



token t;

void parse_DEFINITIONS();
void parse_COMMANDS();
void parse_DEFINITION_TYPE();
void parse_DEFINITION_MAYBE();
void parse_DEFINITION();
void parse_CONDITION();
void parse_COMMAND_MAYBE();
void parse_COMMAND();
void parse_ID_LIST();

extern FILE *log_file;

void dbl_puts(char *str) {
    puts(str);
    fprintf(log_file, "%s\n", str);
}

void parse_PROGRAM()
{
    dbl_puts("START_PROG DEFINITIONS START_COM COMMANDS END_PROG");
	match(START_PROG);
	parse_DEFINITIONS();
	match(START_COM);
	parse_COMMANDS();
	match(END_PROG);
	match(EOF);
}

void parse_DEFINITIONS()
{
	dbl_puts("DEFINITIONS -> DEFINITION DEFINITION_MAYBE");
	parse_DEFINITION();
	parse_DEFINITION_MAYBE();
}

int follow_DEFINITION_TYPE(token *t) {
    return t->type == SEMICOLON || t->type == START_COM;
}

int follow_DEFINITION_MAYBE(token *t) {
    return t->type == SEMICOLON || t->type == START_COM;
}

int follow_DEFINITION(token *t) {
    return t->type == SEMICOLON || t->type == START_COM;
}

int follow_ID_LIST_MAYBE(token *t) {
    return t->type == SEMICOLON || t->type == START_COM;
}

int follow_ID_LIST(token *t) {
    return t->type == SEMICOLON || t->type == START_COM;
}

int follow_COMMAND_MAYBE(token *t) {
    return t->type == END_PROG || t->type == WHILE || t->type == END_EMBED || t->type == ELSE || t->type == END_IF;
}

int follow_COMMAND(token *t) {
   return t->type == END_PROG || t->type == WHILE || t->type == END_EMBED || t->type == SEMICOLON || t->type == ELSE || t->type == END_IF;;
}

int follow_TYPE(token *t) {
    return t->type == ID || t->type == CONST;
}

int follow_EXPRESSION_SIMPLE(token *t) {
    return t->type == END_PROG ||
           t->type == WHILE    ||
           t->type == END_EMBED ||
           t->type == SEMICOLON ||
           t->type == RIGHT_BRACKET ||
           t->type == AR_OP ||
           t->type == END_LOOP ||
           t->type == ELSE ||
           t->type == END_IF ||
           t->type == THEN ||
           t->type == REL_OP;
}

int follow_EXPRESSION_MAYBE(token *t) {
    return t->type == END_PROG || t->type == WHILE || t->type == END_EMBED || t->type == SEMICOLON || t->type == RIGHT_BRACKET || t->type == END_LOOP || t->type == ELSE || t->type == END_IF || t->type == THEN || t->type == REL_OP;;
}

int follow_EXPRESSION(token *t) {
    return t->type == END_PROG || t->type == WHILE || t->type == END_EMBED || t->type == SEMICOLON || t->type == RIGHT_BRACKET || t->type == END_LOOP || t->type == ELSE || t->type == END_IF || t->type == THEN || t->type == REL_OP;;
}

void error_handler(int (*follow)(token *t), char *message) {
	printf("Error in line %d - %s, but got %s\n", t.line, message, token_name(t.type));
    fprintf(log_file, "Error in line %d - %s, but got %s\n", t.line, message, token_name(t.type));
    for (; !follow(&t) && t.type != EOF; t = next_token()); 
	back_token();
}

void parse_DEFINITION_MAYBE()
{
	t = next_token();
	switch (t.type)
	{
	case SEMICOLON:
		dbl_puts("DEFINITION_MAYBE -> ; DEFINITIONS");
		parse_DEFINITIONS();
		break;
	case START_COM:
		dbl_puts("DEFINITION_MAYBE -> eps");
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
	switch (t.type)
	{
	case ID:
		dbl_puts("DEFINITION_TYPE -> ID_LIST");
		back_token();
		parse_ID_LIST();
		break;
	case CONST:
		match(ID);
		match(ASSIGN);
		t = next_token();
		if (t.type != INT_LITERAL && t.type != REAL_LITERAL)
			error_handler(follow_DEFINITION_TYPE, "expected number");
		break;
	default:
		error_handler(follow_DEFINITION_TYPE, "expected const or id");
	}
}

void parse_DEFINITION()
{
	t = next_token();
	switch (t.type) {
	case ID:
		dbl_puts("DEFINITION -> id : exception");
		match(COLON);
		match(EXCEPTION);
		break;
	case INT:
	case REAL:
		dbl_puts("DEFINITION -> TYPE DEFINITION_TYPE");
		parse_DEFINITION_TYPE();
		break;
	default:
		error_handler(follow_DEFINITION, "expected int or real");
	}
}


void parse_TYPE()
{
	t = next_token();
	switch(t.type)
	{
	case INT:
		dbl_puts("TYPE -> integer");
		break;
	case REAL:
		dbl_puts("TYPE -> real");
		break;
	default:
		error_handler(follow_TYPE, "expected integer or real");
	}
}

void parse_ID_LIST_MAYBE() {
    t = next_token();
	switch (t.type) {
	case COMMA:
		dbl_puts("ID_LIST_MAYBE -> , id ID_LIST_MAYBE");
		match(ID);
		parse_ID_LIST_MAYBE();
		break;
	case SEMICOLON:
	case START_COM:
		dbl_puts("ID_LIST_MAYBE -> eps");
		back_token();
		break;
	default:
		error_handler(follow_ID_LIST_MAYBE, "expected comma");
	}
}

void parse_ID_LIST() {
    t = next_token();
	switch (t.type)
	{
	case ID:
		dbl_puts("ID_LIST -> id ID_LIST_MAYBE");
		parse_ID_LIST_MAYBE();
		break;
	default:
		error_handler(follow_ID_LIST, "expected id");
	}
}

void parse_COMMANDS() {
	dbl_puts("COMMANDS -> COMMAND COMMAND_MAYBE");
	parse_COMMAND();
	parse_COMMAND_MAYBE();
}

void parse_COMMAND_MAYBE() {
	t = next_token();
	switch (t.type) 
	{
	case SEMICOLON:
		dbl_puts("COMMAND_MAYBE -> ; COMMANDS");
		parse_COMMANDS();
		break;
	case END_PROG:
	case ELSE:
	case END_IF:
	case WHILE:
	case END_EMBED:
		dbl_puts("COMMAND_MAYBE -> eps");
		back_token();
		break;
	default:
		error_handler(follow_COMMAND_MAYBE, "expected SEMICOLON");
	}
}

void parse_EXPRESSION();

void parse_COMMAND() {
	t = next_token();
	switch (t.type) {
	case ID:
		dbl_puts("COMMAND -> id := EXPRESSION");
		match(ASSIGN);
		parse_EXPRESSION();
		break;
    case IF:
        dbl_puts("COMMAND -> if CONDITION then COMMANDS else COMMANDS end_if");
        parse_CONDITION();
        match(THEN);
        parse_COMMANDS();
        match(ELSE);
        parse_COMMANDS();
        match(END_IF);
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
	switch (t.type)
	{
	case INT_LITERAL:
		dbl_puts("EXPRESSION_SIMPLE -> NUMBER");
		break;
	case REAL_LITERAL:
		dbl_puts("EXPRESSION_SIMPLE -> NUMBER");
		break;
	case ID:
		dbl_puts("EXPRESSION_SIMPLE -> id");
		break;
	case LEFT_BRACKET:
		dbl_puts("EXPERSSION_SIMPLE -> (EXPRESSION)");
		parse_EXPRESSION();
		match(RIGHT_BRACKET);
		break;
	default:
		error_handler(follow_EXPRESSION_SIMPLE, "expected number or id or (");
	}
}

void parse_EXPRESSION_MAYBE();

void parse_EXPRESSION() {
	dbl_puts("EXPRESSION -> EXPRESSION_SIMPLE EXPRESSION_MAYBE");
	parse_EXPRESSION_SIMPLE();
	parse_EXPRESSION_MAYBE();
}

void parse_EXPRESSION_MAYBE() 
{
	t = next_token();
	switch (t.type)
	{
	case AR_OP:
		dbl_puts("EXPRESSION_MAYBE -> AR_OP EXPRESSION");
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
		dbl_puts("EXPRESSION_MAYBE -> eps");
        back_token();
		break;
	default:
		error_handler(follow_EXPRESSION_MAYBE, "expected AR_OP");
	}
}

void parse_CONDITION()
{
	dbl_puts("CONDITION -> EXPRESSION REL_OP EXPRESSION");
	parse_EXPRESSION();
	match(REL_OP);
	parse_EXPRESSION();
}
