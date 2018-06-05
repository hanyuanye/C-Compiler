#include <fstream>
#include <string>
#include <iostream>
#include "lexer.h"
#include "Parser.h"
#include "AsmGenerator.h"
int main(int argc, char** argv)
{
	std::string s = "int main() {return (1+1)/2;}";
	Lexer lexer;
	lexer.lex(s);
	Parser parser(lexer.getTokens());
//	for (auto const& t: lexer.getTokens()) {
//		std::cout << t.first << " " << t.second.type << " " << t.second.value << std::endl;
//	}

	parser.parse();
//	parser.print();
	AsmGenerator asmGen;
	asmGen.generateAssembly(parser.ast);
	asmGen.emit();
}
