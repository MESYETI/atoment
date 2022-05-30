#pragma once
#include "_components.hh"
#include "lexer.hh"

#define ATM_BOOLEAN_TRUE  1
#define ATM_BOOLEAN_FALSE 0

namespace ATM {
	struct Language_Components;
	
	typedef void (*CXXFunction)(Language_Components&);

	enum class FunctionType {
		CXXFunction,
		ATMFunction
	};

	struct Function {
		FunctionType               type;
		CXXFunction                cxxFunction;
		std::vector <Lexer::Token> atmFunction;
		size_t                     atmFunctionSize;
	};

	struct FunctionMapItem {
		std::string name;
		Function    function;
	};

	class FunctionMap {
		public:
			// variables
			std::vector <FunctionMapItem> map;

			// functions
			FunctionMap();
			~FunctionMap();

			// util functions
			Function& Get(std::string name);
			bool      Exists(std::string name);
			void      AddCXXFunction(std::string name, CXXFunction function);
			void      AddATMFunction(std::string name, std::vector <Lexer::Token> function, size_t size);
	};
	
	struct Language_Components {
		std::vector <int8_t>       stack;
		std::vector <int8_t>       returnStack;
		FunctionMap                functionMap;
		std::vector <Lexer::Token> tokens;
		size_t                     tokenIterator;
		size_t                     forLoopIterator;
		std::vector <std::string>  programArgv;
	};

	Language_Components BuildLanguageComponents(std::vector <Lexer::Token> tokens, std::vector <std::string> argv);
	Language_Components CopyLanguageComponents(Language_Components lc);
}
