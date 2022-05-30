#include "app.hh"
#include "fs.hh"
#include "lexer.hh"
#include "interpreter.hh"

App::App(int argc, char** argv) {
	for (int i = 0; i < argc; ++i) {
		args.push_back(argv[i]);
	}

	if (args.size() <= 1) {
		printf("Usage: %s [script]\n", args[0].c_str());
		exit(0);
	}

	bool   gotFlags        = false;
	bool   showTokens      = false;
	bool   showStack       = false;
	size_t programArgStart = 0;
	for (size_t i = 1; i < args.size(); ++i) {
		if ((args[i][0] == '-') && !gotFlags) {
			if ((args[i] == "--show-tokens") || (args[i] == "-st")) {
				showTokens = true;
			}
			else if ((args[i] == "--show-stack") || (args[i] == "-ss")) {
				showStack = true;
			}
		}
		else {
			if (!gotFlags) {
				programArgStart = i;
				scriptFileName  = args[i];
			}
			gotFlags = true;
		}
	}

	for (size_t i = programArgStart; i < args.size(); ++i) {
		programArgs.push_back(args[i]);
	}

	if (!FS::File::Exists(scriptFileName)) {
		fprintf(stderr, "[ERROR] file '%s' doesn't exist\n", scriptFileName.c_str());
		exit(1);
	}

	scriptTokens = Lexer::Lex(FS::File::Read(scriptFileName));

	if (showTokens) {
		for (size_t i = 0; i < scriptTokens.size(); ++i) {
			printf("[%i] %s\n", (int) i, Lexer::StringifyToken(scriptTokens[i]).c_str());
		}
		exit(0);
	}

	languageComponents = ATM::BuildLanguageComponents(scriptTokens, programArgs);

	Interpret(scriptTokens, languageComponents);

	if (showStack) {
		puts("stack debug");
		for (size_t i = 0; i < languageComponents.stack.size(); ++i) {
			printf("[%i]: %i\n", (int) i, languageComponents.stack[i]);
		}
	}
}

App::~App() {
	
}
