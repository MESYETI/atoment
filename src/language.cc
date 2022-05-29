#include "language.hh"
#include "builtin.hh"

ATM::FunctionMap::FunctionMap() {
	
}

ATM::FunctionMap::~FunctionMap() {
	
}

ATM::Function& ATM::FunctionMap::Get(std::string name) {
	for (size_t i = 0; i < map.size(); ++i) {
		if (map[i].name == name) {
			return map[i].function;
		}
	}
	fprintf(stderr, "[ERROR] tried to get unknown function '%s'\n", name.c_str());
	exit(1);
}

bool ATM::FunctionMap::Exists(std::string name) {
	for (size_t i = 0; i < map.size(); ++i) {
		if (map[i].name == name) {
			return true;
		}
	}
	return false;
}

void ATM::FunctionMap::AddCXXFunction(std::string name, ATM::CXXFunction function) {
	ATM::Function add;
	add.type            = ATM::FunctionType::CXXFunction;
	add.cxxFunction     = function;
	add.atmFunctionSize = 0;
	map.push_back({name, add});
}

void ATM::FunctionMap::AddATMFunction(std::string name, std::vector <Lexer::Token> function, size_t size) {
	ATM::Function add;
	add.type            = ATM::FunctionType::ATMFunction;
	add.atmFunction     = function;
	add.atmFunctionSize = size;
	map.push_back({name, add});
}

ATM::Language_Components ATM::BuildLanguageComponents(std::vector <Lexer::Token> tokens) {
	ATM::Language_Components ret;

	ret.tokens          = tokens;
	ret.tokenIterator   = 0;
	ret.forLoopIterator = 0;

	ret.functionMap.AddCXXFunction(".",       BuiltIn::PrintInt);
	ret.functionMap.AddCXXFunction("putstr",  BuiltIn::PutStr);
	ret.functionMap.AddCXXFunction("putch",   BuiltIn::PutCh);
	ret.functionMap.AddCXXFunction("if",      BuiltIn::If);
	ret.functionMap.AddCXXFunction("func",    BuiltIn::Func);
	ret.functionMap.AddCXXFunction("include", BuiltIn::Include);
	ret.functionMap.AddCXXFunction("for",     BuiltIn::For);
	ret.functionMap.AddCXXFunction("i",       BuiltIn::I);
	ret.functionMap.AddCXXFunction("+",       BuiltIn::Add);
	ret.functionMap.AddCXXFunction("-",       BuiltIn::Sub);
	ret.functionMap.AddCXXFunction("*",       BuiltIn::Mul);
	ret.functionMap.AddCXXFunction("/",       BuiltIn::Div);
	ret.functionMap.AddCXXFunction("%",       BuiltIn::Mod);
	ret.functionMap.AddCXXFunction("dup",     BuiltIn::Dup);

	return ret;
}

ATM::Language_Components ATM::CopyLanguageComponents(Language_Components lc) {
	ATM::Language_Components ret;

	ret.stack           = lc.stack;
	ret.returnStack     = lc.stack;
	ret.functionMap     = lc.functionMap;
	ret.tokens          = lc.tokens;
	ret.tokenIterator   = 0;
	ret.forLoopIterator = 0;

	return ret;
}
