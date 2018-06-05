#include "AsmGenerator.h"

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

void AsmGenerator::generateAssembly(std::shared_ptr<AstNode> ast) {
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

