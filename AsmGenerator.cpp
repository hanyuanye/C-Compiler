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

std::string AsmGenerator::getOffset(std::string identifier) {
	auto offset = variableMap[scopeCount].find(identifier);
	if (offset == variableMap[scopeCount].end()) {
		error("unknown identifier");
	}
	return std::to_string((*offset).second);
}

void AsmGenerator::generateAssembly(std::shared_ptr<AstNode> ast) {
	if (!ast) return;

	if (ast->type == "function") {
		varStackIndex = 0;
		emitln(std::string(".globl ") + ast->value);
		emitln(std::string(ast->value + ":"));
		emitln("push	%ebp");
		emitln("movl	%esp, %ebp");
		varStackIndex = 0;
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
		std::string offset = getOffset(ast->value);
		emitln(std::string("movl	%eax, ") + offset  + "(%ebp)");
	}

	if (ast->type == "identAccess") {
		std::string offset = getOffset(ast->value);
		emitln(std::string("movl	") + offset + "(%ebp), %eax");
	}

	if (ast->type == "numDecls") {
		currStack = -4 *std::stoi(ast->value);
	}

	if (ast->type == "decl" ) {
		for (const auto &map : variableMap) {
			if (map.find(ast->value) != map.end()) {
				error("redefinition of variable");
			}
		}
		variableMap[scopeCount].insert(std::make_pair(ast->value, varStackIndex));
		varStackIndex -= 4;
	}

	if (ast->type == "boolOp") {
		if (ast->value == "||") {
			push("%eax", 4);
			generateAsmChildren(ast);
			pop("%ecx");
			emitln("orl	%ecx, %eax");
			emitln("movl	$0, %eax");
			emitln("setne	%al");
		}
		if (ast->value == "&&") {
			push("%eax", 4);
			generateAsmChildren(ast);
			pop("%ecx");
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
			push("%eax", 4);
			generateAsmChildren(ast);
			pop("%ecx");
			emitln("cmpl	%ecx, %eax");
			emitln("movl	$0, %eax");
			emitln("sete	%al");
		}

		if (ast->value == "!=") {
			push("%eax", 4);
			generateAsmChildren(ast);
			pop("%ecx");
			emitln("cmpl	%ecx, %eax");
			emitln("movl	$0, %eax");
			emitln("setne	%al");
		}

		return;
	}

	if (ast->type == "relOp") {
		if (ast->value == ">") {
			push("%eax", 4);
			generateAsmChildren(ast);
			pop("%ecx");
			emitln("cmpl	%ecx, %eax");
			emitln("movl	$0, %eax");
			emitln("setl	%al");
		}

		if (ast->value == "<") {
			push("%eax", 4);
			generateAsmChildren(ast);
			pop("%ecx");
			emitln("cmpl	%ecx, %eax");
			emitln("movl	$0, %eax");
			emitln("setg	%al");
		}
		if (ast->value == "<=") {
			push("%eax", 4);
			generateAsmChildren(ast);
			pop("%ecx");
			emitln("cmpl	%ecx, %eax");
			emitln("movl	$0, %eax");
			emitln("setge	%al");
		}
		if (ast->value == ">=") {
			push("%eax", 4);
			generateAsmChildren(ast);
			pop("%ecx");
			emitln("cmpl	%ecx, %eax");
			emitln("movl	$0, %eax");
			emitln("setle	%al");
		}

		return;
	}

	if (ast->type == "addOp") {
		if (ast->value == "+") {
			push("%eax", 4);
			generateAsmChildren(ast);
			pop("%ecx");
			emitln("addl	%ecx, %eax");
		}

		if (ast->value == "-") {
			push("%eax", 4);
			generateAsmChildren(ast);
			pop("%ecx");
			emitln("subl	%eax, %ecx");
			emitln("movl	%ecx, %eax");
		}

		return;
	}
	
	if (ast->type == "mulOp") {
		if (ast->value == "*") {
			push("%eax", 4);
			generateAsmChildren(ast);
			pop("%ecx");
			emitln("imul	%ecx, %eax");
		}

		if (ast->value == "/") {
			push("%eax", 4);
			generateAsmChildren(ast);
			pop("%ecx");
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

void AsmGenerator::push(std::string value, int increment) {
	currStack -= increment;
	emitln(std::string("movl	") + value + ", " + std::to_string(currStack)  + "(%ebp)");
	increments.push_back(increment);
}

void AsmGenerator::pop(std::string value) {
	if (increments.empty()) {
		error("stack has no elements");
	}
	emitln(std::string("movl	") + std::to_string(currStack)  + "(%ebp), " + value);
	currStack += increments.back();	
	increments.pop_back();
}
