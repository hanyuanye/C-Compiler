#include <fstream>
#include <string>
#include <iostream>
#include "lexer.h"

int main(int argc, char** argv)
{
	std::string s = "int main() {int a; int aa; return 2; return 3;}";
	Lexer lexer;
	lexer.lex(s);
	for (auto const& t: lexer.getTokens()) {
		std::cout << t.first << " " << t.second.type << " " << t.second.value << std::endl;
	}
}
