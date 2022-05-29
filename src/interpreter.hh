#pragma once
#include "_components.hh"
#include "lexer.hh"
#include "language.hh"

void Interpret(std::vector <Lexer::Token> tokens, ATM::Language_Components& lc);
