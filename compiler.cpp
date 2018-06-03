#include <fstream>
#include <string>
#include <iostream>
#include "lexer.h"
#include "generateAST.h"

int main(int argc, char** argv)
{
	std::string s = "int main() {return 2;}";
	Lexer lexer;
	lexer.lex(s);
	Parser parser(lexer.getTokens());
	for (auto const& t: lexer.getTokens()) {
		std::cout << t.first << " " << t.second.type << " " << t.second.value << std::endl;
	}

	parser.parse();
	parser.print();
}
