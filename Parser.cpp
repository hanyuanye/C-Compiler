#include "Parser.h"
#include <fstream>
#include <string>
#include <iostream>
#include "InputStream.h"
Parser::Parser() {
	is = NULL;
}

Parser::Parser(InputStream _is) {
	is = new InputStream(_is);
}

Parser::~Parser() {
	delete is;	
}

void Parser::factor() {
	if (is->look == '(') {
		is->match('(');
		expression();
		is->match(')');
	}
	else if (is->isAlpha(is->look)) {
		ident();
	}
	else {
		is->emitln(std::string("MOVE #") + is->getNum() + ",D0");
	}
}

void Parser::ident() {
	std::string name = is->getName();
	if (is->look == '(') {
		is->match('(');
		is->match(')');
		is->emitln(std::string("BSR ") + name);
	} 
	else {
		is->emitln(std::string("MOVE ") + name + "(PC),D0");
	}
}

void Parser::multiply() {
	is->match('*');
	factor();
	is->emitln("MULS (SP)+,D0");
}

void Parser::divide() {
	is->match('/');
	factor();
	is->emitln("MOVE (SP)+, D1");
	is->emitln("DIVS D1, D0");
}

void Parser::term() {
	factor();
	while (is->isMulOp()) {
		is->emitln("MOVE D0,-(SP)");
		switch (is->look) {
			case '*':
				multiply();
				break;
			case '/':
				divide();
				break;
			default:
				is->expect("Mulop");
				break;
		};
	}	
}

void Parser::add() {
	is->match('+');
	term();
	is->emitln("ADD (SP)+,D0");
}

void Parser::subtract() {
	is->match('-');
	term();
	is->emitln("SUB (SP)+, D0");
	is->emitln("NEG D0");
}

void Parser::expression() {
	if (is->isAddOp()) {
		is->emitln("CLR D0");
	} else {
		term();
	}
	if (!is->isAddOp()) is->expect("AddOps");
	while (is->isAddOp()) {
		is->emitln("MOVE D0,-(SP)");
		switch(is->look) {
			case '+':
				add();
				break;
			case '-':
				subtract();
				break;
			default:
				is->expect("AddOp");
				break;
		};
	}  
}

void Parser::assign() {
	std::string name = is->getName();
	is->match('=');
	expression();
	is->emitln(std::string("LEA") + name + "(PC),A0");
	is->emitln("MOVE D0,(A0)");
}
