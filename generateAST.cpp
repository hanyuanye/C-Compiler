#include "generateAST.h"

const std::string statementArr[] = {"return"};
const std::unordered_set<std::string> Parser::statement(statementArr, statementArr + sizeof(statementArr)/sizeof(statementArr[0]));

Parser::Parser(std::map<int, Token> _tokenList) {
	tokenList = _tokenList;
	look = tokenList.begin();
}

Token Parser::getNext() {
	if (look == tokenList.end()) {
		return Token("end", INT_MAX);
	}
	Token t = (*look).second;
	look++;
	return t;
};

bool Parser::contains(const std::unordered_set<std::string> dictionary, std::string value) {
	return (dictionary.find(value) != dictionary.end());
}

void Parser::expect(std::string s) {
	std::cout << "Expected: " + s << std::endl;
}

void Parser::abort(std::string s) {
	expect(s);
	exit(0);
}

void Parser::parseStatement() {
	Token t = getNext();
	if (contains(statement, t.type)) {
		std::cout << "hi" << std::endl;
	} 	
}
