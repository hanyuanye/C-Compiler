#include "AsmGenerator.h"

AsmGenerator::AsmGenerator() {
	scopeCount = 0;
	currStack = 0;
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
	for (auto i = variableMap.begin(); i != variableMap.end(); i++) {
		if ((*i).find(identifier) != (*i).end()) {
			return std::to_string(-4 + (*(*i).find(identifier)).second);
		}
	}
	error(identifier + ": unknown identifier");
	return "";
}

void AsmGenerator::generateAssembly(std::shared_ptr<AstNode> ast) {
	if (!ast) return;
	std::string offset;
	if (ast->type == "function") {
		pushScope();
		varStackIndex = 0;
		paramIndex = 12;
		branchCount = 0;
		variableMap.push_back(std::unordered_map<std::string, int>());
		if (!addFunction(ast)) {
			error("redefinition of function");
		}
		emitln(std::string(".globl ") + ast->value);
		emitln(std::string(ast->value + ":"));
		emitln("push	%ebp");
		emitln("movl	%esp, %ebp");
		generateAsmChildren(ast);
		popScope();
		return;
	}

	if (ast->type == "funcTerm") {
		if (functionMap.find(ast->value) == functionMap.end()) {
			error("unidentified function call");
		}
		//TODO: implement parameter passing
		generateAsmChildren(ast);
		emitln(std::string("call	" + ast->value));
		return;
	}

	if (ast->type == "parameterFunctionPrototype") {
		for (const auto &map : variableMap) {
			if (map.find(ast->value) != map.end()) {
				error("redefinition of variable");
			}
		}
		variableMap[scopeCount].insert(std::make_pair(ast->value, paramIndex));
		paramIndex += 4;
	}

	if (ast->type == "parameter") {
		generateAsmChildren(ast);
		emitln("push	%eax");
		return;
	}

	if (ast->type == "const") {
		emitln(std::string("movl  $") + ast->value + ", %eax");
	}

	if (ast->type == "statement") {
		if (ast->value == "return") {
			emitln("leave");
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
		int bytes = 4 * std::stoi(ast->value);
		emitln(std::string("sub   $") + std::to_string(bytes) + ", %esp");
		currStack += -4 *std::stoi(ast->value);
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

	if(ast->type == "whileBlock") {
		emitln(std::string("_branch") + pushBranch() + ":");
	}

	if (ast->type == "whileBody") {
		pushScope();
		generateAsmChildren(ast);
		popScope();
		return;
	}

	if (ast->type == "whileElseBody") {
		emitln(std::string("_branch") + popBranch() + ":");
	}

	if (ast->type == "loop") {
		emitln("jmp	_branch" + popSecondBranch());
	}

	if (ast->type == "ifBlock") {
		emitln("cmpl	$0, %eax");
		emitln("je	_branch" + pushBranch());
	}

	if (ast->type == "ifBody") {
		pushScope();
		generateAsmChildren(ast);
		popScope();
		emitln("jmp	_branch" + pushBranch());
		return;
	}

	if (ast->type == "elseBody") {
		pushScope();
		emitln(std::string("_branch") + popSecondBranch() + ":");
		generateAsmChildren(ast);
		popScope();
		emitln(std::string("_branch") + popBranch() + ":");
		return;
	}

	if (ast->type == "boolOp") {
		if (ast->value == "||") {
			emitln("push    %eax");
			generateAsmChildren(ast);
			emitln("pop    %ecx");
			emitln("orl	%ecx, %eax");
			emitln("movl	$0, %eax");
			emitln("setne	%al");
		}
		if (ast->value == "&&") {
			emitln("push    %eax");
			generateAsmChildren(ast);
			emitln("pop    %ecx");
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
			emitln("push    %eax");
			generateAsmChildren(ast);
			emitln("pop    %ecx");
			emitln("cmpl	%ecx, %eax");
			emitln("movl	$0, %eax");
			emitln("sete	%al");
		}

		if (ast->value == "!=") {
			emitln("push    %eax");
			generateAsmChildren(ast);
			emitln("pop    %ecx");
			emitln("cmpl	%ecx, %eax");
			emitln("movl	$0, %eax");
			emitln("setne	%al");
		}

		return;
	}

	if (ast->type == "relOp") {
		if (ast->value == ">") {
			emitln("push    %eax");
			generateAsmChildren(ast);
			emitln("pop    %ecx");
			emitln("cmpl	%ecx, %eax");
			emitln("movl	$0, %eax");
			emitln("setl	%al");
		}

		if (ast->value == "<") {
			emitln("push    %eax");
			generateAsmChildren(ast);
			emitln("pop    %ecx");
			emitln("cmpl	%ecx, %eax");
			emitln("movl	$0, %eax");
			emitln("setg	%al");
		}
		if (ast->value == "<=") {
			emitln("push    %eax");
			generateAsmChildren(ast);
			emitln("pop    %ecx");
			emitln("cmpl	%ecx, %eax");
			emitln("movl	$0, %eax");
			emitln("setge	%al");
		}
		if (ast->value == ">=") {
			emitln("push    %eax");
			generateAsmChildren(ast);
			emitln("pop    %ecx");
			emitln("cmpl	%ecx, %eax");
			emitln("movl	$0, %eax");
			emitln("setle	%al");
		}

		return;
	}

	if (ast->type == "addOp") {
		if (ast->value == "+") {
			emitln("push    %eax");
			generateAsmChildren(ast);
			emitln("pop    %ecx");
			emitln("addl	%ecx, %eax");
		}

		if (ast->value == "-") {
			emitln("push    %eax");
			generateAsmChildren(ast);
			emitln("pop    %ecx");
			emitln("subl	%eax, %ecx");
			emitln("movl	%ecx, %eax");
		}

		return;
	}
	
	if (ast->type == "mulOp") {
		if (ast->value == "*") {
			emitln("push    %eax");
			generateAsmChildren(ast);
			emitln("pop    %ecx");
			emitln("imul	%ecx, %eax");
		}

		if (ast->value == "/") {
			emitln("push    %eax");
			generateAsmChildren(ast);
			emitln("pop    %ecx");
			emitln("push    %eax");
			emitln("movl	%ecx, %eax");
			emitln("pop    %ecx");
			emitln("xor	%edx, %edx");
			emitln("idivl	%ecx");
		}

		if (ast->value == "%") {
			emitln("push    %eax");
			generateAsmChildren(ast);
			emitln("pop    %ecx");
			emitln("push    %eax");
			emitln("movl	%ecx, %eax");
			emitln("pop    %ecx");
			emitln("xor	%edx, %edx");
			emitln("idivl	%ecx");
			emitln("movl	%edx, %eax");
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

bool AsmGenerator::addFunction(std::shared_ptr<AstNode> node) {
	if (functionMap.find(node->value) != functionMap.end()) {
		return false;
	}
	functionMap.insert(node->value);
	return true;
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

std::string AsmGenerator::pushBranch() {
	branchCount++;
	branchStack.push_back(branchCount);
	return std::to_string(branchCount);
}

std::string AsmGenerator::popBranch() {
	int branch = branchStack.back();
	branchStack.pop_back();
	return std::to_string(branch);
}

std::string AsmGenerator::popSecondBranch() {
	int branch = branchStack.at(branchStack.size() - 2);
	branchStack.erase(branchStack.begin() + branchStack.size() - 2);
	return std::to_string(branch);
}

void AsmGenerator::pushScope() {
	scopeCount++;
	variableMap.push_back(std::unordered_map<std::string, int>());
}

void AsmGenerator::popScope() {
	scopeCount--;
	variableMap.pop_back();
}
