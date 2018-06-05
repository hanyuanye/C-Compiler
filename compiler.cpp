#include <fstream>
#include <string>
#include <iostream>
#include "lexer.h"
#include "Parser.h"
#include "AsmGenerator.h"

std::string readFile(std::string fileString) {
	std::ifstream file(fileString);
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string code = buffer.str();
	return code;
}
int main(int argc, char** argv)
{
	std::string code = readFile("test.txt");
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
