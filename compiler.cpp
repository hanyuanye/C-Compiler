#include <fstream>
#include <string>
#include <iostream>
#include "lexer.h"
#include "Parser.h"
std::string code;

void emitln(std::string s) {
	code += s + "\n";
	std::cout << s << std::endl;
}

void emitcode() {
	std::ofstream asmm;
	asmm.open("assembly.s");
	asmm << code;
	asmm.close();
}

void generateAssembly(std::shared_ptr<AstNode> ast) {
	if (!ast) return;
	if (ast->type == "function") {
		emitln(std::string(".globl ") + ast->value);
		emitln(std::string(ast->value + ":"));
	}
	if (ast->type == "const") {
		emitln(std::string("movl  $") + ast->value + ", %eax");
	}
	if (ast->type == "statement") {
		emitln("ret");
	}
	for (auto i = ast->children.rbegin(); i != ast->children.rend(); i++) {
		generateAssembly(*i);
	}
}

int main(int argc, char** argv)
{
	std::string s = "int main() {return 42;}";
	Lexer lexer;
	lexer.lex(s);
	Parser parser(lexer.getTokens());
//	for (auto const& t: lexer.getTokens()) {
//		std::cout << t.first << " " << t.second.type << " " << t.second.value << std::endl;
//	}

	parser.parse();
//	parser.print();

	generateAssembly(parser.ast);
	emitcode();
}
