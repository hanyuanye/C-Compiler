#include "Parser.h"

const std::string statementArr[] = {"return", "if", "while"};
const std::unordered_set<std::string> Parser::statementDict(statementArr, statementArr + sizeof(statementArr)/sizeof(statementArr[0]));
const std::string addOpArr[] = {"+","-"};
const std::unordered_set<std::string> Parser::addOpDict(addOpArr, addOpArr + sizeof(addOpArr)/sizeof(addOpArr[0]));
const std::string mulOpArr[] = {"*","/"};
const std::unordered_set<std::string> Parser::mulOpDict(mulOpArr, mulOpArr + sizeof(mulOpArr)/sizeof(mulOpArr[0]));
const std::string unaryArr[] = {"-","~","!"};
const std::unordered_set<std::string> Parser::unaryDict(unaryArr, unaryArr + sizeof(unaryArr)/sizeof(unaryArr[0]));
const std::string relOpArr[] = {"<", ">", "<=", ">="};
const std::unordered_set<std::string> Parser::relOpDict(relOpArr, relOpArr+sizeof(relOpArr)/sizeof(relOpArr[0]));
const std::string equalityOpArr[] = {"==", "!="};
const std::unordered_set<std::string> Parser::equalityOpDict(equalityOpArr, equalityOpArr+sizeof(equalityOpArr)/sizeof(equalityOpArr[0]));
const std::string typeArr[] = {"int"};
const std::unordered_set<std::string> Parser::typeDict(typeArr, typeArr+sizeof(typeArr)/sizeof(typeArr[0]));

Parser::Parser(std::map<int, Token> _tokenList) {
	tokenList = _tokenList;
	look = tokenList.begin();
}

Token Parser::getNext() {
	if (look == tokenList.end()) {
		return Token("end", INT_MAX);
	}
	Token t = (*look).second;
	look++;
	return t;
}

std::string Parser::peekNext() {
	return (*look).second.type;
}

void Parser::abortMatch(std::string match) {
	Token t = getNext();
	if (t.type != match) {
		abort(match);
	}
}

bool Parser::match(std::string match, std::string type) {
	return type == match;
}

bool Parser::contains(const std::unordered_set<std::string> dictionary, std::string value) {
	return (dictionary.find(value) != dictionary.end());
}

void Parser::expect(std::string s) {
	std::cout << "Expected: " + s << std::endl;
}

void Parser::abort(std::string s) {
	expect(s);
	exit(0);
}

void Parser::parse() {
	AstNode function = parseFunction();
	AstNode program("program", function);
	ast = std::make_shared<AstNode>(program);
}

AstNode Parser::parseFunction() {
	AstNode function("function");
	abortMatch("int");
	Token t = getNext();
	if (t.type != "identifier") {
		abort("identifier");
	}
	function.addValue(t.value);
	abortMatch("(");
	abortMatch(")");
	abortMatch("{");
	AstNode body = parseBody();
	if (!body.scopeFlag) {
		AstNode statementBlock("statementBlock");
		AstNode ret("statement", "return");
		statementBlock.addNode(ret);
		statementBlock.addNode(AstNode("const", "0"));
		body.addNodeFront(statementBlock);
	}
	abortMatch("}");
	function.addNode(body);
	return function;
}

AstNode Parser::parseBody() {
	AstNode body("body");
	AstNode line;
	bool returnInScope = false;
	int declarations = 0;
	while (contains(statementDict, peekNext()) || contains(typeDict, peekNext()) || match("identifier", peekNext())) {
		if (contains(statementDict, peekNext())) {
			if (match("return", peekNext())) {
				returnInScope = true;
				line = parseReturnStatement();
			}
			else if (match("if", peekNext())) {
				line = parseIfStatement();
			}
			else if (match("while", peekNext())) {
				line = parseWhileStatement();
			}
		}
		else if (contains(typeDict, peekNext())) {
			declarations++;
			line = parseDecl();
		}
		else if (match("identifier", peekNext())) {
			line = parseAssign(getNext().value);
		}
		body.addNodeFront(line);
	}
	AstNode numDecls("numDecls", std::to_string(declarations));
	body.addNode(numDecls);
	body.scopeFlag = returnInScope;
	return body;
}

AstNode Parser::parseWhileStatement() {
	AstNode whileBlock("whileBlock");
	AstNode ifBlock = parseIfStatement();
	whileBlock.addNode(ifBlock);
	return whileBlock;
}

AstNode Parser::parseIfStatement() {
	AstNode ifBlock("ifBlock");
	AstNode ifStatement("ifOp");
	Token t = getNext();
	abortMatch("(");
	AstNode condition = parseExpression();
	abortMatch(")");
	abortMatch("{");
	AstNode ifBody = parseBody();
	ifBody.type = "ifBody";
	abortMatch("}");
	if (match("else", peekNext())) {
		Token t = getNext();
		abortMatch("{");
		AstNode elseBody = parseBody();
		elseBody.type = "elseBody";
		ifBlock.addNode(elseBody);
		abortMatch("}");
	}
	else {
		AstNode elseBody("elseBody");
		ifBlock.addNode(elseBody);
	}
	ifBlock.addNode(ifBody);
	ifStatement.addNode(ifBlock);
	ifStatement.addNode(condition);
	return ifStatement;
}

AstNode Parser::parseDecl() {
	AstNode decl("declaration");
	Token t = getNext();
	t = getNext();
	if (!match("identifier", t.type)) {
		abort("identifier");
	}
	AstNode variable("decl", t.value);
	if (match("=", peekNext())) {
		AstNode assign = parseAssign(t.value);
		decl.addNode(assign);
	}
	decl.addNode(variable);
	abortMatch(";");
	return decl;
}

AstNode Parser::parseAssign(std::string value) {
	AstNode assign("assignOp", "=");
	Token t = getNext(); //incrementing by one token to pass assingment operator
	AstNode assignVar("identAssign", value);
	AstNode expr = parseExpression(); //getting expression to parse with
	assign.addNode(assignVar);
	assign.addNode(expr);//since in post order, want to identify expression first when generating assembly
	return assign;
}

AstNode Parser::parseReturnStatement() {
	AstNode statementBlock("statementBlock");
	Token t = getNext();
	if (!contains(statementDict, t.type)) {
		abort("statement");
	}
	AstNode exp = parseExpression();
	AstNode statement("statement", t.type);
	statementBlock.addNode(statement);
	statementBlock.addNode(exp);
	abortMatch(";");
	return statementBlock;
}

AstNode Parser::parseExpression() {
	AstNode expr("expression");
	AstNode boolTerm = parseAnd();
	while (match("||", peekNext())) {
		AstNode op("boolOp", getNext().type);
		AstNode nextTerm = parseAnd();
		op.addNode(nextTerm);
		expr.addNode(op);
	}
	expr.addNode(boolTerm);
	return expr;
}

AstNode Parser::parseAnd() {
	AstNode boolTerm("boolTerm");
	AstNode equalityTerm = parseEquality();
	while (match("&&", peekNext())) {
		AstNode op("boolOp", getNext().type);
		AstNode nextTerm;
		if (match("(", peekNext())) {
			nextTerm = parseExpression();
			abortMatch(")");
		}
		else {
			nextTerm = parseEquality();
		}
		op.addNode(nextTerm);
		boolTerm.addNode(op);
	}
	boolTerm.addNode(equalityTerm);
	return boolTerm;
}

AstNode Parser::parseEquality() {
	AstNode equalityTerm("equalityTerm");
	AstNode relTerm = parseRelational();
	while (contains(equalityOpDict, peekNext())) {
		AstNode op("equalityOp", getNext().type);
		AstNode nextTerm = parseRelational();
		op.addNode(nextTerm);
		equalityTerm.addNode(op);
	}
	equalityTerm.addNode(relTerm);
	return equalityTerm;
}

AstNode Parser::parseRelational() {
	AstNode relationalTerm("relationalTerm");
	AstNode arith = parseArithmetic();
	while (contains(relOpDict, peekNext())) {
		AstNode op("relOp", getNext().type); 
		AstNode nextArith = parseArithmetic();
		op.addNode(nextArith);
		relationalTerm.addNode(op);
	}
	relationalTerm.addNode(arith);
	return relationalTerm;
}

AstNode Parser::parseArithmetic() {
	AstNode expr("arithmetic");
	AstNode term = parseTerm();
	while (contains(addOpDict, peekNext())) {
		AstNode op("addOp", getNext().type);
		AstNode nextTerm = parseTerm();
		op.addNode(nextTerm);
		expr.addNode(op);
	}
	expr.addNode(term);
	return expr;
}

AstNode Parser::parseTerm() {
	AstNode term("term");
	AstNode factor = parseFactor();
	while (contains(mulOpDict, peekNext())) {
		AstNode op("mulOp", getNext().type);
		AstNode nextFactor = parseFactor();
		op.addNode(nextFactor);
		term.addNode(op);
	}
	term.addNode(factor); 	
	return term;
}

AstNode Parser::parseFactor() {
	Token t = getNext();
	if (match("(", t.type)) {
		AstNode expr = parseExpression();
		abortMatch(")");
		return expr;
	}
	else if (match("number", t.type)) {
		return AstNode("const",t.value);
	}
	else if (match("identifier", t.type)) {
		return AstNode("identAccess", t.value);
	}
	else if (contains(unaryDict, t.type)) {
		AstNode expr = parseFactor();
		return UnOp(t.type, expr);
	}
	abort("expression");
}

AstNode Parser::UnOp(std::string type, AstNode expr) {
	expr.addNode(AstNode("unary", type));
	return expr;
}

void Parser::print() {
	printHelper(ast);	
}

void Parser::printHelper(std::shared_ptr<AstNode> &node) {
	if (!node) return;
	std::cout << node->type << std::endl;
	for (auto &child : node->children) {
		printHelper(child);
	}
}
