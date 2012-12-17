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

#ifndef TOKEN_HEADER
#define TOKEN_HEADER
typedef struct {
	int type;
	char *text;
	int line;
} token;

token next_token();
void back_token();

#define WHILE 200
#define START_COM 201
#define RAISE 202
#define LOOP 203
#define ID 204
#define END_EMBED 205
#define EMBED 206
#define END_LOOP 207
#define ASSIGN 208
#define SEMICOLON 209
#define THEN 210
#define START_PROG 211
#define RIGHT_BRACKET 212
#define REL_OP 213
#define REAL_LITERAL 214
#define REAL 215
#define INT 216
#define INT_LITERAL 217
#define END_PROG 218
#define LEFT_BRACKET 219
#define EXCEPTION 220
#define COLON 221
#define COMMA 222
#define END_IF 223
#define ELSE 224
#define AR_OP 225
#define IF 226
#define CONST 227
#endif
