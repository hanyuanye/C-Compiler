#include <fstream>
#include <string>
#include <iostream>
#include "InputStream.h"

void term(InputStream *is) {
	is->emitln(std::string("MOVE #") + is->getNum() + ",D0");
}

void add(InputStream *is) {
	is->match('+');
	term(is);
	is->emitln("ADD D1,D0");
}

void subtract(InputStream *is) {
	is->match('-');
	term(is);
	is->emitln("SUB D1, D0");
	is->emitln("NEG D0");
}

void expression(InputStream *is) {
	term(is);
	is->emitln("MOVE D0,D1");
	switch(is->look) {
		case '+':
			add(is);
			break;
		case '-':
			subtract(is);
		default:
			break;
	};
}

int main(int argc, char** argv)
{
	std::string s = "1";
	InputStream* is = new InputStream(s);
	expression(is);
}
