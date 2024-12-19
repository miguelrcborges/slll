#ifndef SLLC_PARSER_C_H

#include "../c.h"

enum TokenTypes {
	TOKEN_INVALID,
	TOKEN_PRIMITIVE_TYPE,
	TOKEN_IDENTIFIER,
	TOKEN_TYPE,

	TOKEN_LBRACE,
	TOKEN_RBRACE,
	TOKEN_LBRACKET,
	TOKEN_RBRACKET,
	TOKEN_AMPERSAND,
	TOKEN_EOF,
};


typedef struct Token {
	u8 type;
	u32 value_index;
} Token;


typedef struct Lexer {
	String source;
	usize position;
	usize token_line;
	Token current_token;
} Lexer;

void LexNextToken(Lexer *lex);

#define SLLC_PARSER_C_H
#endif
