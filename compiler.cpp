#include <fstream>
#include <string>
#include <iostream>
#include "lexer.h"
#include "Parser.h"
#include "AsmGenerator.h"
int main(int argc, char** argv)
{
	std::string code = "int main() {return (1+1)/2;}"; //edit this string to edit source code being compiled
	Lexer lexer;
	lexer.lex(code);
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
