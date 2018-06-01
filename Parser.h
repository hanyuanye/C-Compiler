#ifndef PARSER_H
#define PARSER_H

#include "InputStream.h"

class Parser {
public:
	Parser();
	~Parser();
	Parser(InputStream _is);
	void expression();

private:
	InputStream* is;
	void factor();
	void multiply();
	void divide();
	void add();
	void subtract();
	void term();


};
#endif
