#ifndef GENERATEAST_H
#define GENERATEAST_H

#include <iostream>
#include "lexer.h"

class Parser {
	private:
		Parser(std::map<int, Token> _tokenList);
		std::map<int, Token> tokenList;

	public:
		void expected(std::string s);
		void parse
};


#endif
