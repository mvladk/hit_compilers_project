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

#include <stdlib.h>
#include <string.h>
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

token next_token() {
	extern int line_number;
	extern int yylex();
	extern char *yytext;

	if (did_back_token) {
        did_back_token = 0;
		return *current_token;
    }
	else {
		token *t;
		delete_token(previous_token);
		previous_token = current_token;
		t = (token*)malloc(sizeof(token));
		t->type = yylex();
		t->text = strdup(yytext);
		t->line = line_number;
		current_token = t;
		return *t;
	}
}
