#include "c.h"
#include <stdio.h>

static void LexSkipWhitespace(Lexer *lex);

void LexNextToken(Lexer *lex) {
	LexSkipWhitespace(lex);
	if (lex->position >= lex->source.length) {
		lex->current_token = (Token) {
			.type = TOKEN_EOF,
		};
		return;
	}

	fputc(lex->source.buffer[lex->position], stdout);
	lex->position += 1;
}


static void LexSkipWhitespace(Lexer *lex) {
	while (
		lex->position < lex->source.length &&
		u8IsSpace(lex->source.buffer[lex->position])
	) {
		lex->position += 1;
	}
}
