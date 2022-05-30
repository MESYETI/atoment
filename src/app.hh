#pragma once
#include "_components.hh"
#include "lexer.hh"
#include "language.hh"

class App {
	public:
		// variables
		std::vector <std::string>  args;
		std::vector <std::string>  programArgs;
		std::string                scriptFileName;
		std::vector <Lexer::Token> scriptTokens;
		ATM::Language_Components   languageComponents;

		// functions
		App(int argc, char** argv);
		~App();
};
