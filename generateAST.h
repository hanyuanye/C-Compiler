#ifndef GENERATEAST_H
#define GENERATEAST_H

#include <iostream>
#include <unordered_set>
#include <limits.h>
#include <cstdlib>
#include "lexer.h"

class Parser {
	private:
		Token getNext();
		std::map<int, Token> tokenList;
		void expect(std::string s);
		void abort(std::string s);
		bool contains(std::unordered_set<std::string> dictionary, std::string val);
		std::map<int, Token>::iterator look;

		static const std::unordered_set<std::string> statement;
	public:
		Parser(std::map<int, Token> _tokenList);
		void parseStatement();
};


#endif
