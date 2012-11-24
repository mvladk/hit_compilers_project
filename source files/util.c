#include <stdio.h>
#include "token.h"

char *token_name(int token_type)
{
	switch (token_type)
	{
	case INT:
		return "int";
	case REAL:
		return "real";
	case LEFT_BRACKET:
		return "LEFT_BRACKET";
	}
}

void match(int token_type)
{
	token *t = next_token();
	if (t->type != token_type) {
		printf("At line %d - expected %s\n", t->line, token_name(token_type));
		return;
	}
}