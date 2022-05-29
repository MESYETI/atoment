#include "lexer.hh"
#include "util.hh"

std::vector <Lexer::Token> Lexer::Lex(std::string script) {
	std::vector <Lexer::Token> ret;
	bool                       inString = false;
	std::string                reading;
	size_t                     cols = 1;
	size_t                     lines = 1;

	for (size_t i = 0; i <= script.length(); ++i) {
		if (script[i] == '\n') {
			++ lines;
			cols = 1;
		}
		else {
			++ cols;
		}
		switch (script[i]) {
			case '"': {
				if (inString) {
					inString = false;
					ret.push_back({
						Lexer::TokenType::String,
						reading,
						cols,
						lines
					});
					reading = "";
					break;
				}
				else if (reading == "") {
					inString = true;
					break;
				}
				else {
					reading += script[i];
				}
				break;
			}
			case '\\': {
				if (inString) {
					++ i;
					if (script[i] == '\0') {
						fprintf(stderr, "[ERROR] incomplete string at end of file");
						exit(1);
					}
					switch (script[i]) {
						case 'n': {
							reading += '\n';
							break;
						}
					}
				}
				break;
			}
			case '/': { // comment
				if (inString) {
					reading += script[i];
					break;
				}
				if (script[i + 1] != '/') {
					break;
				}
				while ((script[i] != '\n') && (script[i] != '\0')) {
					++ i;
				}
				break;
			}
			case '\0':
			case '\n':
			case '\t':
			case ' ': {
				if (inString) {
					reading += script[i];
					break;
				}

				if (Util::IsNumber(reading)) {
					ret.push_back({
						Lexer::TokenType::Integer,
						reading,
						cols,
						lines
					});
				}
				else if (reading != "") {
					ret.push_back({
						Lexer::TokenType::Word,
						reading,
						cols,
						lines
					});
				}
				reading = "";
				break;
			}
			default: {
				reading += script[i];
				break;
			}
		}
	}

	if (inString) {
		fprintf(stderr, "[ERROR] incomplete string at end of file\n");
		exit(1);
	}

	return ret;
}

std::string Lexer::StringifyToken(Lexer::Token token) {
	switch (token.type) {
		case Lexer::TokenType::Word: {
			return "(Word) '" + token.content + "'";
		}
		case Lexer::TokenType::String: {
			return "(String) '" + token.content + "'";
		}
		case Lexer::TokenType::Integer: {
			return "(Integer) '" + token.content + "'";
		}
	}
	return "";
}
