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

#include "symbol.h"
#include "token.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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


int parse_TYPE();

table_ptr head = NULL;
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
    head = make_table(head);
    dbl_puts("START_PROG DEFINITIONS START_COM COMMANDS END_PROG");
	match(START_PROG);
	parse_DEFINITIONS();
	match(START_COM);
	parse_COMMANDS();
	match(END_PROG);
	match(EOF);
    head = pop_table(head);
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

void parse_DEFINITION_TYPE(int type)
{
	t = next_token();
	switch (t.type)
	{
	case ID:
		dbl_puts("DEFINITION_TYPE -> ID_LIST");
		back_token();
		parse_ID_LIST(type);
		break;
	case CONST:
	{
		table_entry e;
		t = match(ID);
        e = insert(head, t.text);
        if (!e) {
            printf("Error in line %d - %s already defined\n", t.line, t.text);
            fprintf(log_file, "Error in line %d - %s already defined\n", t.line, t.text);
        } else {
            set_id_type(e, type);
            set_id_role(e, CONSTANT_ROLE);
        }
		match(ASSIGN);
		t = next_token();
		if (t.type != INT_LITERAL && t.type != REAL_LITERAL)
			error_handler(follow_DEFINITION_TYPE, "expected number");
        else if (e && get_id_type(e) == INT_TYPE && t.type == REAL_LITERAL) {
            printf("Error in line %d - cannot asssign real to int\n", t.line);
            fprintf(log_file, "Error in line %d - cannot asssign real to int\n", t.line);
		}
		break;
	}
	default:
		error_handler(follow_DEFINITION_TYPE, "expected const or id");
	}
}

void parse_DEFINITION()
{
   table_entry e;
    elm_type type;
	t = next_token();
	switch (t.type) {
	case ID:
        e = insert(head, t.text);
        if (e) {
            set_id_type(e, ERROR_TYPE);
            set_id_role(e, EXCEPTION_ROLE);
        } else {
            printf("Error in line %d - %s is already defined\n", t.line, t.text);
            fprintf(log_file, "Error in line %d - %s is already defined\n", t.line, t.text);
        }
		dbl_puts("DEFINITION -> id : exception");
		match(COLON);
		match(EXCEPTION);
		break;
	case INT:
	case REAL:
        back_token();
        type = parse_TYPE();        
		dbl_puts("DEFINITION -> TYPE DEFINITION_TYPE");
		parse_DEFINITION_TYPE(type);
		break;
	default:
		error_handler(follow_DEFINITION, "expected int or real");
	}
}


int parse_TYPE()
{
	t = next_token();
	switch(t.type)
	{
	case INT:
		dbl_puts("TYPE -> integer");
        return INT_TYPE;
	case REAL:
		dbl_puts("TYPE -> real");
        return REAL_TYPE;
	default:
		error_handler(follow_TYPE, "expected integer or real");
        return ERROR_TYPE;
	}
}

void parse_ID_LIST_MAYBE(int type) {
	table_entry e;
    t = next_token();
	switch (t.type) {
	case COMMA:
		dbl_puts("ID_LIST_MAYBE -> , id ID_LIST_MAYBE");
		t = match(ID);
        e = insert(head, t.text);
        if (e) {
            set_id_type(e, type);
            set_id_role(e, VARIABLE_ROLE);
        } else {
            printf("Error in line %d - %s is already defined\n", t.line, t.text);
            fprintf(log_file, "Error in line %d - %s is already defined\n", t.line, t.text);
        }
		parse_ID_LIST_MAYBE(type);
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

void parse_ID_LIST(int type) {
    table_entry e;
    t = next_token();
	switch (t.type)
	{
	case ID:
        e = insert(head, t.text);
        if (e) {
            set_id_type(e, type);
            set_id_role(e, VARIABLE_ROLE);
        } else {
            printf("Error in line %d - %s is already defined\n", t.line, t.text);
            fprintf(log_file, "Error in line %d - %s is already defined\n", t.line, t.text);
        }
		dbl_puts("ID_LIST -> id ID_LIST_MAYBE");
		parse_ID_LIST_MAYBE(type);
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

elm_type parse_EXPRESSION();

void parse_COMMAND() {
	table_entry e;
	int type;
    char *id_name;
	t = next_token();
	switch (t.type) {
	case ID:
        id_name = strdup(t.text);
		dbl_puts("COMMAND -> id := EXPRESSION");
		match(ASSIGN);
		type = parse_EXPRESSION();
        e = find(head, id_name);
        if (!e) {
            printf("Error in line %d - %s not defined\n", t.line, id_name);
            fprintf(log_file, "Error in line %d - %s not defined\n", t.line, id_name);
        } else if (get_id_role(e) != VARIABLE_ROLE) {
            printf("Error in line %d - cannot assign to non variable %s\n", t.line, id_name);
            fprintf(log_file, "Error in line %d - cannot assign to non variable %s\n", t.line, id_name);
        } else if (get_id_type(e) == INT_TYPE &&
                   type == REAL_TYPE) {
            printf("Error in line %d - cannot assign real to int %s\n", t.line, id_name);
            fprintf(log_file, "Error in line %d - cannot assign real to int %s\n", t.line, id_name);
        }
        free(id_name);
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
        head = make_table(head);
		parse_DEFINITIONS();
		match(START_COM);
		parse_COMMANDS();
        head = pop_table(head);
		match(END_EMBED);
		break;
	case RAISE:
		t = match(ID);
        e = find(head, t.text);
        if (!e) {
            printf("Error in line %d - undefined exception %s\n", t.line, t.text);
            fprintf(log_file, "Error in line %d - undefined exception %s\n", t.line, t.text);
        } else if (get_id_role(e) != EXCEPTION_ROLE) {
            printf("Error in line %d - %s is not an exception_role\n", t.line, t.text);
            fprintf(log_file, "Error in line %d - %s is not an exception_role\n", t.line, t.text);
        }
		break;
	default:
		error_handler(follow_COMMAND, "expected id or if or LOOP or embed or RAISE");
	}
}

int parse_EXPRESSION_SIMPLE()
{
	elm_type return_type;
	table_entry e;
	t = next_token();
	switch (t.type)
	{
	case INT_LITERAL:
		dbl_puts("EXPRESSION_SIMPLE -> NUMBER");
        return INT_TYPE;
	case REAL_LITERAL:
		dbl_puts("EXPRESSION_SIMPLE -> NUMBER");
        return REAL_TYPE;
	case ID:
		dbl_puts("EXPRESSION_SIMPLE -> id");
        e = find(head, t.text);
        if (!e) {
            printf("Error in line %d - undefined variable %s\n", t.line, t.text);
            fprintf(log_file, "Error in line %d - undefined variable %s\n", t.line, t.text);
            return ERROR_TYPE;
        } else if (get_id_role(e) == EXCEPTION_ROLE)
        {
            printf("Error in line %d - %s should be constant or variable, not exception\n", t.line, t.text);
            fprintf(log_file, "Error in line %d - %s should be constant or variable, not exception\n", t.line, t.text);
            return ERROR_TYPE;
        }
        else
        {
            return get_id_type(e);
        }
		break;
	case LEFT_BRACKET:
		dbl_puts("EXPERSSION_SIMPLE -> (EXPRESSION)");
		return_type = parse_EXPRESSION();
		match(RIGHT_BRACKET);
        return return_type;
		break;
	default:
		error_handler(follow_EXPRESSION_SIMPLE, "expected number or id or (");
        return ERROR_TYPE;
	}
}

elm_type parse_EXPRESSION_MAYBE();

elm_type parse_EXPRESSION() {
	elm_type type1, type2;
	dbl_puts("EXPRESSION -> EXPRESSION_SIMPLE EXPRESSION_MAYBE");
	type1 = parse_EXPRESSION_SIMPLE(); 
	type2 = parse_EXPRESSION_MAYBE();
    if (type1 == REAL_TYPE || type2 == REAL_TYPE)
        return REAL_TYPE;
    else if (type1 == INT_TYPE && type2 == INT_TYPE)
        return INT_TYPE;
    else if (type2 == EPSILON_TYPE)
        return type1;
    else
        return ERROR_TYPE;
}

elm_type parse_EXPRESSION_MAYBE() 
{
	t = next_token();
	switch (t.type)
	{
	case AR_OP:
		dbl_puts("EXPRESSION_MAYBE -> AR_OP EXPRESSION");
		return parse_EXPRESSION();
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
        return EPSILON_TYPE;
	default:
		error_handler(follow_EXPRESSION_MAYBE, "expected AR_OP");
        return EPSILON_TYPE;
	}
}

void parse_CONDITION()
{
	dbl_puts("CONDITION -> EXPRESSION REL_OP EXPRESSION");
	parse_EXPRESSION();
	match(REL_OP);
	parse_EXPRESSION();
}
