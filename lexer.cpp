#include "lexer.h"

Lexer::Lexer() {
	typeRegex = std::regex("return|int|;|\\(|\\)|\\{|\\}|[_a-zA-Z][_a-zA-Z0-9]{0,30}*|-|~|!|\\+|\\/|\\*|&&|\\|\\||<[=>]?|==|>=?|\\&\\&|\\|\\||=|if|else");
	numRegex = std::regex("[0-9]+");
	std::vector<std::string> typeArr = {
		"return",
		"int",
		";",
		"(",
		")",
		"{",
		"}",
		"+",
		"-",
		"*",
		"/",
		"~",
		"!",
		"+",
		"*",
		"/",
		"&&",
		"||",
		">",
		">=",
		"<",
		"<=",
		"==",
		"=",
		"if",
		"else"
	};
	for (const auto& s : typeArr) {
		typeDictionary.insert(s);
	}
}

void Lexer::lex(std::string &file) {
	tokenList.clear();
	std::string s = file;
	auto typeBegin = std::sregex_iterator(s.begin(), s.end(), typeRegex);
	auto typeEnd = std::sregex_iterator();
	for (std::sregex_iterator i = typeBegin; i != typeEnd; i++) {
		std::smatch match = *i;
		Token t;
		if (typeDictionary.find(match.str()) != typeDictionary.end()) {
			t = Token(match.str(), match.position());
		} 
		else {
			t = Token("identifier", match.str(), match.position());
		}
		tokenList.insert(std::pair<int, Token>(match.position(), t));
	}

	auto numBegin = std::sregex_iterator(s.begin(), s.end(), numRegex);
	for (auto i = numBegin; i != typeEnd; i++) {
		std::smatch match = *i;
		Token t("number", match.str(), match.position());
		tokenList.insert(std::pair<int, Token>(match.position(), t));
	}
}

std::map<int, Token> Lexer::getTokens() {
	return tokenList;
}

