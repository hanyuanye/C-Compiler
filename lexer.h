#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <iostream>
#include <map>
#include <unordered_set>
#include <regex>
#include <vector>
struct Token {
	std::string type;
	std::string value;
	int pos;
	Token(std::string _type, std::string _value, int _pos) : 
		type(_type), value(_value), pos(_pos) {}
	Token(std::string _type, int _pos) : type(_type), pos(_pos){}
	Token(){}		
};

class Lexer {
	private:
		std::map<int, Token> tokenList;
		std::unordered_set<std::string> typeDictionary;
		std::regex typeRegex;
		std::regex identifierRegex;
		std::regex numRegex;
	public:
		Lexer();
		void lex(std::string &file);
		std::map<int, Token> getTokens();
};

#endif
