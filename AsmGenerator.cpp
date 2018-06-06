#include "AsmGenerator.h"

AsmGenerator::AsmGenerator() {
	scopeCount = 0;
	variableMap.push_back(std::unordered_map<std::string, int>());
}

void AsmGenerator::emitln(std::string s) {
	code += s + "\n";
	std::cout << s << std::endl;
}

void AsmGenerator::emit() {
	std::ofstream asmm;
	asmm.open("assembly.s");
	asmm << code;
	asmm.close();
}

void AsmGenerator::error(std::string msg) {
	std::cout << msg << std::endl;
	exit(0);
}

void AsmGenerator::generateAssembly(std::shared_ptr<AstNode> ast) {
	if (!ast) return;

	if (ast->type == "function") {
		stackIndex = 0;
		emitln(std::string(".globl ") + ast->value);
		emitln(std::string(ast->value + ":"));
		emitln("push	%ebp");
		emitln("movl	%esp, %ebp");
		stackIndex = -4;
	}

	if (ast->type == "const") {
		emitln(std::string("movl  $") + ast->value + ", %eax");
	}

	if (ast->type == "statement") {
		if (ast->value == "return") {
			emitln("movl	%ebp, %esp");
			emitln("pop	%ebp");
			emitln("ret");
		}
	}

	if (ast->type == "identAssign") {
		auto offset = variableMap[scopeCount].find(ast->value);
		emitln(std::string("movl	%eax, ") + std::to_string((*offset).second) + "(%ebp)");
	}

	if (ast->type == "identAccess") {
		auto offset = variableMap[scopeCount].find(ast->value);
		emitln(std::string("movl	") + std::to_string((*offset).second) + "(%ebp), %eax");
	}

	if (ast->type == "decl" ) {
		for (const auto &map : variableMap) {
			if (map.find(ast->value) != map.end()) {
				error("redefinition of variable");
			}
		}
		variableMap[scopeCount].insert(std::make_pair(ast->value, stackIndex));
		stackIndex -= 4;
	}

	if (ast->type == "boolOp") {
		if (ast->value == "||") {
			emitln("push	%eax");
			generateAsmChildren(ast);
			emitln("pop	%ecx");
			emitln("orl	%ecx, %eax");
			emitln("movl	$0, %eax");
			emitln("setne	%al");
		}
		if (ast->value == "&&") {
			emitln("push	%eax");
			generateAsmChildren(ast);
			emitln("pop	%ecx");
			emitln("cmpl	$0, %ecx");
			emitln("setne	%cl");
			emitln("cmpl	$0, %eax");
			emitln("movl	$0, %eax");
			emitln("setne	%al");
			emitln("andb	%cl, %al");
		}
		return;
	}

	if (ast->type == "equalityOp") {
		if (ast->value == "==") {
			emitln("push	%eax");
			generateAsmChildren(ast);
			emitln("pop	%ecx");
			emitln("cmpl	%ecx, %eax");
			emitln("movl	$0, %eax");
			emitln("sete	%al");
		}

		if (ast->value == "!=") {
			emitln("push	%eax");
			generateAsmChildren(ast);
			emitln("pop	%ecx");
			emitln("cmpl	%ecx, %eax");
			emitln("movl	$0, %eax");
			emitln("setne	%al");
		}

		return;
	}

	if (ast->type == "relOp") {
		if (ast->value == ">") {
			emitln("push	%eax");
			generateAsmChildren(ast);
			emitln("pop	%ecx");
			emitln("cmpl	%ecx, %eax");
			emitln("movl	$0, %eax");
			emitln("setl	%al");
		}

		if (ast->value == "<") {
			emitln("push	%eax");
			generateAsmChildren(ast);
			emitln("pop	%ecx");
			emitln("cmpl	%ecx, %eax");
			emitln("movl	$0, %eax");
			emitln("setg	%al");
		}
		if (ast->value == "<=") {
			emitln("push	%eax");
			generateAsmChildren(ast);
			emitln("pop	%ecx");
			emitln("cmpl	%ecx, %eax");
			emitln("movl	$0, %eax");
			emitln("setge	%al");
		}
		if (ast->value == ">=") {
			emitln("push	%eax");
			generateAsmChildren(ast);
			emitln("pop	%ecx");
			emitln("cmpl	%ecx, %eax");
			emitln("movl	$0, %eax");
			emitln("setle	%al");
		}

		return;
	}

	if (ast->type == "addOp") {
		if (ast->value == "+") {
			emitln("push	%eax");
			generateAsmChildren(ast);
			emitln("pop	%ecx");
			emitln("addl	%ecx, %eax");
		}

		if (ast->value == "-") {
			emitln("push	%eax");
			generateAsmChildren(ast);
			emitln("pop	%ecx");
			emitln("subl	%eax, %ecx");
			emitln("movl	%ecx, %eax");
		}

		return;
	}
	
	if (ast->type == "mulOp") {
		if (ast->value == "*") {
			emitln("push	%eax");
			generateAsmChildren(ast);
			emitln("pop	%ecx");
			emitln("imul	%ecx, %eax");
		}

		if (ast->value == "/") {
			emitln("push	%eax");
			generateAsmChildren(ast);
			emitln("pop	%ecx");
			emitln("push	%eax");
			emitln("movl	%ecx, %eax");
			emitln("pop	%ecx");
			emitln("xor	%edx, %edx");
			emitln("idivl	%ecx");
		}

		return;
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

	generateAsmChildren(ast);
}

void AsmGenerator::generateAsmChildren(std::shared_ptr<AstNode> ast) {
	for (auto i = ast->children.rbegin(); i != ast->children.rend(); i++) {
		generateAssembly(*i);
	}
}

