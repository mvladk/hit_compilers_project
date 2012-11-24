#include <stdlib.h>
#include "token.h"

token *current_token = 0, *previous_token = 0;
int did_back_token = 0;
void delete_token(token *t) {
	if (t)
	{
		free(t->text);
		free(t);
	}
}

void back_token() 
{ 
	did_back_token = 1; 
}

token *next_token() {
	extern int line_number;
	extern int yylex();
	extern char *yytext;

	if (did_back_token)
		return current_token;
	else {
		token *t;
		delete_token(previous_token);
		previous_token = current_token;
		t = (token*)malloc(sizeof(token));
		t->type = yylex();
		t->text = yytext;
		t->line = line_number;
		current_token = t;
		return t;
	}
}
