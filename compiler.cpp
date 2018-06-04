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
	if (ast->type == "unary") {
		if (ast->value == "-") {
			emitln("neg	%eax");
		} 
		else if (ast->value == "!") {
			emitln("cmpl	$0, %eax");
			emitln("movl	$0, %eax");
			emitln("sete	%al");
		}
		else if (ast->value == "~") {
			emitln("not	%eax");
		}
	}
	for (auto i = ast->children.rbegin(); i != ast->children.rend(); i++) {
		generateAssembly(*i);
	}
}

int main(int argc, char** argv)
{
	std::string s = "int main() {return !0;}";
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
