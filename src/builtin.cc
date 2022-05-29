#include "builtin.hh"
#include "interpreter.hh"
#include "fs.hh"

void BuiltIn::PrintInt(ATM::Language_Components& lc) { // .
	printf("%i", lc.stack.back());
	lc.stack.pop_back();
}

void BuiltIn::PutStr(ATM::Language_Components& lc) {
	if (lc.stack.size() == 0) {
		fprintf(stderr, "[ERROR] putstr: stack underflow (stack empty from function call)\n");
		exit(1);
	}
	while (lc.stack.back() != 0) {
		putchar(lc.stack.back());
		lc.stack.pop_back();
		if ((lc.stack.back() != 0) && (lc.stack.size() == 0)) {
			fprintf(stderr, "[ERROR] putstr: stack underflow\n");
			exit(1);
		}
	}
}

void BuiltIn::PutCh(ATM::Language_Components& lc) {
	putchar(lc.stack.back());
	lc.stack.pop_back();
}

void BuiltIn::If(ATM::Language_Components& lc) {
	size_t start = lc.tokenIterator + 1;
	size_t end   = start;
	{
		size_t counter = 1;
		for (size_t i = end; i < lc.tokens.size(); ++i) {
			if (lc.tokens[i].type == Lexer::TokenType::Word) {
				if (
					(lc.tokens[i].content == "if") ||
					(lc.tokens[i].content == "func ")
				) {
					++ counter;
				}
				else if (lc.tokens[i].content == "end") {
					-- counter;
				}
				if (counter == 0) {
					end = i;
					break;
				}
			}
		}
		if (counter != 0) {
			fprintf(stderr, "[ERROR] incomplete if statement\n");
			exit(1);
		}
	}

	if (lc.stack.back() != ATM_BOOLEAN_FALSE) {
		lc.stack.pop_back();
		std::vector <Lexer::Token> block;
		
		for (size_t i = start; i < end; ++i) {
			block.push_back(lc.tokens[i]);
		}

		ATM::Language_Components newlc = ATM::CopyLanguageComponents(lc);

		Interpret(block, newlc);

		lc = ATM::CopyLanguageComponents(newlc); // copy updates to the stack etc
	}
	lc.stack.pop_back();
	lc.tokenIterator = end;
}

void BuiltIn::Func(ATM::Language_Components& lc) {
	if (lc.tokens.size() < lc.tokenIterator + 1) {
		fprintf(stderr, "[ERROR] function definition missing name\n");
		exit(1);
	}

	++ lc.tokenIterator;
	//puts(Lexer::StringifyToken(lc.tokens[lc.tokenIterator]).c_str());
	if (lc.tokens[lc.tokenIterator].type != Lexer::TokenType::Word) {
		fprintf(stderr, "[ERROR] invalid function name: '%s'\n", lc.tokens[lc.tokenIterator].content.c_str());
		exit(1);
	}
	if (lc.functionMap.Exists(lc.tokens[lc.tokenIterator].content)) {
		fprintf(stderr, "[ERROR] attempted to define new function under the name of an already existing function: '%s'\n", lc.tokens[lc.tokenIterator].content.c_str());
		exit(1);
	}

	std::string functionName = lc.tokens[lc.tokenIterator].content;

	++ lc.tokenIterator;
	if (lc.tokenIterator == lc.tokens.size()) {
		fprintf(stderr, "[ERROR] incomplete function definition\n");
		exit(1);
	}

	//puts(Lexer::StringifyToken(lc.tokens[lc.tokenIterator]).c_str());

	size_t start = lc.tokenIterator;
	size_t end;

	{
		size_t counter = 1;
		for (size_t i = start; i < lc.tokens.size(); ++i) {
			if (
				(lc.tokens[i].content == "if") ||
				(lc.tokens[i].content == "func ")
			) {
				++ counter;
			}
			else if (lc.tokens[i].content == "end") {
				-- counter;
			}
			if (counter == 0) {
				end = i;
				break;
			}
		}
		if (counter != 0) {
			fprintf(stderr, "[ERROR] incomplete function definition\n");
			exit(1);
		}
	}

	std::vector <Lexer::Token> block;

	for (size_t i = start; i < end; ++i) {
		if ((lc.tokens[i].type == Lexer::TokenType::Word) && (lc.tokens[i].content == "end")) {
			continue;
		}
		block.push_back(lc.tokens[i]);
	}

	lc.functionMap.AddATMFunction(functionName, block, end - start);

	lc.tokenIterator = end;
}

void BuiltIn::Include(ATM::Language_Components& lc) {
	std::string path;

	if (lc.stack.size() == 0) {
		fprintf(stderr, "[ERROR] include: stack underflow (stack empty from function call)\n");
		exit(1);
	}
	while (lc.stack.back() != 0) {
		//putchar(lc.stack.back());
		path += lc.stack.back();
		lc.stack.pop_back();
		if ((lc.stack.back() != 0) && (lc.stack.size() == 0)) {
			fprintf(stderr, "[ERROR] include: stack underflow\n");
			exit(1);
		}
	}
	//printf("\n%s\n", path.c_str());

	//printf("including: %s\n", path.c_str());

	std::string includeCode           = FS::File::Read(path);
	std::vector <Lexer::Token> tokens = Lexer::Lex(includeCode);

	ATM::Language_Components newlc;
	newlc        = ATM::CopyLanguageComponents(lc);
	newlc.tokens = lc.tokens;

	//printf("%i\n", (int) tokens.size());

	/*for (size_t i = 0; i < tokens.size(); ++i) {
		printf("[%i] %s\n", (int) i, Lexer::StringifyToken(tokens[i]).c_str());
	}*/

	//exit(0);

	// up to this point, everything is actually perfectly fine
	Interpret(tokens, newlc); // but here, it breaks

	size_t oldIterator = lc.tokenIterator;
	lc = ATM::CopyLanguageComponents(newlc);
	lc.tokenIterator = oldIterator; // if i dont do this then include will be called over and over
}

void BuiltIn::For(ATM::Language_Components& lc) {
	if (lc.stack.size() < 2) {
		fprintf(stderr, "[ERROR] for: stack underflow\n");
		exit(1);
	}
	size_t endLoop = lc.stack.back();
	lc.stack.pop_back();
	size_t startLoop = lc.stack.back();
	lc.stack.pop_back();

	size_t start = lc.tokenIterator + 1;
	size_t end   = start;

	{
		size_t counter = 1;
		for (size_t i = end; i < lc.tokens.size(); ++i) {
			if (lc.tokens[i].type == Lexer::TokenType::Word) {
				if (
					(lc.tokens[i].content == "for")
				) {
					//printf("inc: %i\n", (int) counter);
					++ counter;
				}
				else if (lc.tokens[i].content == "loop") {
					-- counter;
					//printf("dec: %i\n", (int) counter);
				}
				if (counter == 0) {
					end = i;
					break;
				}
			}
		}
		if (counter != 0) {
			fprintf(stderr, "[ERROR] incomplete for loop\n");
			exit(1);
		}
	}

	std::vector <Lexer::Token> block;
	
	for (size_t i = start; i < end; ++i) {
		block.push_back(lc.tokens[i]);
	}

	ATM::Language_Components newlc = ATM::CopyLanguageComponents(lc);

	for (uint8_t i = startLoop; i < endLoop; ++i) {
		newlc.forLoopIterator = i;
		Interpret(block, newlc);
	}

	lc = ATM::CopyLanguageComponents(newlc); // copy updates to the stack etc

	lc.tokenIterator = end;
}

void BuiltIn::I(ATM::Language_Components& lc) {
	lc.stack.push_back(lc.forLoopIterator);
}

void BuiltIn::Add(ATM::Language_Components& lc) {
	if (lc.stack.size() < 2) {
		fprintf(stderr, "[ERROR] add: stack underflow\n");
		exit(1);
	}
	uint8_t secondNumber = lc.stack.back();
	lc.stack.pop_back();
	uint8_t firstNumber  = lc.stack.back();
	lc.stack.pop_back();

	lc.stack.push_back(firstNumber + secondNumber);
}

void BuiltIn::Sub(ATM::Language_Components& lc) {
	if (lc.stack.size() < 2) {
		fprintf(stderr, "[ERROR] sub: stack underflow\n");
		exit(1);
	}
	uint8_t secondNumber = lc.stack.back();
	lc.stack.pop_back();
	uint8_t firstNumber  = lc.stack.back();
	lc.stack.pop_back();

	lc.stack.push_back(firstNumber - secondNumber);
}

void BuiltIn::Mul(ATM::Language_Components& lc) {
	if (lc.stack.size() < 2) {
		fprintf(stderr, "[ERROR] mul: stack underflow\n");
		exit(1);
	}
	uint8_t secondNumber = lc.stack.back();
	lc.stack.pop_back();
	uint8_t firstNumber  = lc.stack.back();
	lc.stack.pop_back();

	lc.stack.push_back(firstNumber * secondNumber);
}

void BuiltIn::Div(ATM::Language_Components& lc) {
	if (lc.stack.size() < 2) {
		fprintf(stderr, "[ERROR] div: stack underflow\n");
		exit(1);
	}
	uint8_t secondNumber = lc.stack.back();
	lc.stack.pop_back();
	uint8_t firstNumber  = lc.stack.back();
	lc.stack.pop_back();

	lc.stack.push_back(firstNumber / secondNumber);
}

void BuiltIn::Mod(ATM::Language_Components& lc) {
	if (lc.stack.size() < 2) {
		fprintf(stderr, "[ERROR] mod: stack underflow\n");
		exit(1);
	}
	uint8_t secondNumber = lc.stack.back();
	lc.stack.pop_back();
	uint8_t firstNumber  = lc.stack.back();
	lc.stack.pop_back();

	lc.stack.push_back(firstNumber % secondNumber);
}

void BuiltIn::Dup(ATM::Language_Components& lc) {
	if (lc.stack.size() == 0) {
		fprintf(stderr, "[ERROR] dup: stack underflow\n");
		exit(1);
	}
	lc.stack.push_back(lc.stack.back());
}

void BuiltIn::Equal(ATM::Language_Components& lc) {
	if (lc.stack.size() < 2) {
		fprintf(stderr, "[ERROR] equal: stack underflow\n");
		exit(1);
	}
	uint8_t secondItem = lc.stack.back();
	lc.stack.pop_back();
	uint8_t firstItem  = lc.stack.back();
	lc.stack.pop_back();

	lc.stack.push_back(firstItem == secondItem? ATM_BOOLEAN_TRUE : ATM_BOOLEAN_FALSE);
}

void BuiltIn::LessThan(ATM::Language_Components& lc) {
	if (lc.stack.size() < 2) {
		fprintf(stderr, "[ERROR] greater than: stack underflow\n");
		exit(1);
	}
	uint8_t secondItem = lc.stack.back();
	lc.stack.pop_back();
	uint8_t firstItem  = lc.stack.back();
	lc.stack.pop_back();

	lc.stack.push_back(firstItem < secondItem? ATM_BOOLEAN_TRUE : ATM_BOOLEAN_FALSE);
}

void BuiltIn::GreaterThan(ATM::Language_Components& lc) {
	if (lc.stack.size() < 2) {
		fprintf(stderr, "[ERROR] greater than: stack underflow\n");
		exit(1);
	}
	uint8_t secondItem = lc.stack.back();
	lc.stack.pop_back();
	uint8_t firstItem  = lc.stack.back();
	lc.stack.pop_back();

	lc.stack.push_back(firstItem > secondItem? ATM_BOOLEAN_TRUE : ATM_BOOLEAN_FALSE);
}

void BuiltIn::Not(ATM::Language_Components& lc) {
	if (lc.stack.size() < 1) {
		fprintf(stderr, "[ERROR] not: stack underflow\n");
		exit(1);
	}
	uint8_t boolean = lc.stack.back();
	lc.stack.pop_back();
	lc.stack.push_back(boolean == ATM_BOOLEAN_TRUE? ATM_BOOLEAN_FALSE : ATM_BOOLEAN_TRUE);
}

void BuiltIn::And(ATM::Language_Components& lc) {
	if (lc.stack.size() < 2) {
		fprintf(stderr, "[ERROR] and: stack underflow");
	}

	uint8_t secondItem = lc.stack.back();
	lc.stack.pop_back();
	uint8_t firstItem  = lc.stack.back();
	lc.stack.pop_back();

	lc.stack.push_back((
		(firstItem  == ATM_BOOLEAN_TRUE) &&
		(secondItem == ATM_BOOLEAN_TRUE)
	)? ATM_BOOLEAN_TRUE : ATM_BOOLEAN_FALSE);
}

void BuiltIn::Or(ATM::Language_Components& lc) {
	if (lc.stack.size() < 2) {
		fprintf(stderr, "[ERROR] or: stack underflow");
	}

	uint8_t secondItem = lc.stack.back();
	lc.stack.pop_back();
	uint8_t firstItem  = lc.stack.back();
	lc.stack.pop_back();

	lc.stack.push_back((
		(firstItem  == ATM_BOOLEAN_TRUE) ||
		(secondItem == ATM_BOOLEAN_TRUE)
	)? ATM_BOOLEAN_TRUE : ATM_BOOLEAN_FALSE);
}
