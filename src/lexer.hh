#pragma once
#include "_components.hh"

namespace Lexer {
	enum class TokenType {
		Word,
		String,
		Integer
	};

	struct Token {
		TokenType   type;
		std::string content;
		size_t      col;
		size_t      line;
	};

	std::vector <Token> Lex(std::string script);
	std::string StringifyToken(Token token);
}
