#include "interpreter.hh"

void Interpret(std::vector <Lexer::Token> tokens, ATM::Language_Components& lc) {
	lc.tokens = tokens;
	/*for (size_t i = 0; i < tokens.size(); ++i) {
		printf("[%i] %s\n", (int) i, Lexer::StringifyToken(tokens[i]).c_str());
	}*/
	for (size_t i = 0; i < tokens.size(); ++i) {
		switch (tokens[i].type) {
			case Lexer::TokenType::Integer: {
				lc.stack.push_back(std::stoi(tokens[i].content));
				break;
			}
			case Lexer::TokenType::String: {
				for (ssize_t j = tokens[i].content.length(); j >= 0; --j) {
					lc.stack.push_back(tokens[i].content[j]);
				}
				break;
			}
			case Lexer::TokenType::Word: {
				if (!lc.functionMap.Exists(tokens[i].content)) {
					fprintf(stderr, 
						"[ERROR] at %i:%i: attempt to call non-existant function %s\n", 
						(int)tokens[i].line, (int)tokens[i].col, tokens[i].content.c_str()
					);
					exit(1);
				}
				ATM::Function function = lc.functionMap.Get(tokens[i].content);

				switch (function.type) {
					case ATM::FunctionType::CXXFunction: {
						//printf("cxxfunction: %s\n", tokens[i].content.c_str());
						lc.tokenIterator = i;
						function.cxxFunction(lc);
						i = lc.tokenIterator;
						break;
					}
					case ATM::FunctionType::ATMFunction: {
						ATM::Language_Components newlc = ATM::CopyLanguageComponents(lc);
						Interpret(function.atmFunction, newlc);
						lc = ATM::CopyLanguageComponents(newlc);
						lc.tokenIterator = i + function.atmFunctionSize;
						break;
					}
				}
			}
		}
	}
}
