#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <unordered_set>
#include <limits.h>
#include <cstdlib>
#include <memory>
#include "lexer.h"
#include "AstNode.h"

class Parser {
	private:
		Token getNext();
		std::map<int, Token> tokenList;
		void expect(std::string s);
		void abort(std::string s);
		void abortMatch(std::string s);
		bool contains(std::unordered_set<std::string> dictionary, std::string val);
		std::map<int, Token>::iterator look;

		AstNode parseFunction();
		AstNode parseBody();
		AstNode parseStatement();
		AstNode parseExpression();
		AstNode UnOp(std::string type, AstNode expr);

		void printHelper(std::shared_ptr<AstNode> &node);

		static const std::unordered_set<std::string> statementDict;
		static const std::unordered_set<std::string> operatorDict;
		static const std::unordered_set<std::string> unaryDict;
	public:
		Parser(std::map<int, Token> _tokenList);
		void parse();
		void print();
		std::shared_ptr<AstNode> ast;
		
};


#endif
