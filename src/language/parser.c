#include "c.h"
#include <stdio.h>

#include "lex.c"

void Parse(String source) {
	Lexer lex = {};
	lex.source = source;
	LexNextToken(&lex);

	while (lex.current_token.type != TOKEN_EOF) {
		LexNextToken(&lex);
	}
}
