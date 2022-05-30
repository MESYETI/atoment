#pragma once
#include "_components.hh"
#include "language.hh"

namespace BuiltIn {
	void PrintInt(ATM::Language_Components& lc);    // .
	void PutCh(ATM::Language_Components& lc);       // putch
	void PutStr(ATM::Language_Components& lc);      // putstr
	void If(ATM::Language_Components& lc);          // if
	void Func(ATM::Language_Components& lc);        // func
	void Include(ATM::Language_Components& lc);     // include
	void For(ATM::Language_Components& lc);         // for
	void I(ATM::Language_Components& lc);           // i
	void Add(ATM::Language_Components& lc);         // +
	void Sub(ATM::Language_Components& lc);         // -
	void Mul(ATM::Language_Components& lc);         // *
	void Div(ATM::Language_Components& lc);         // /
	void Mod(ATM::Language_Components& lc);         // %
	void Dup(ATM::Language_Components& lc);         // dup
	void Equal(ATM::Language_Components& lc);       // =
	void LessThan(ATM::Language_Components& lc);    // <
	void GreaterThan(ATM::Language_Components& lc); // >
	void Not(ATM::Language_Components& lc);         // not
	void And(ATM::Language_Components& lc);         // and
	void Or(ATM::Language_Components& lc);          // or
	void GetArg(ATM::Language_Components& lc);      // getarg
	void GetArgSize(ATM::Language_Components& lc);  // getargsize
	void GetAllArgs(ATM::Language_Components& lc);  // getallargs
}
