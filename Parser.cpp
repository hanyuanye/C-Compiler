#include "Parser.h"

const std::string statementArr[] = {"return", "if", "while"};
const std::unordered_set<std::string> Parser::statementDict(statementArr, statementArr + sizeof(statementArr)/sizeof(statementArr[0]));
const std::string addOpArr[] = {"+","-"};
const std::unordered_set<std::string> Parser::addOpDict(addOpArr, addOpArr + sizeof(addOpArr)/sizeof(addOpArr[0]));
const std::string mulOpArr[] = {"*","/", "%"};
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

bool Parser::isEmpty() {
	return look == tokenList.end();
}

std::string Parser::peekNext() {
	return (*look).second.type;
}

void Parser::abortMatch(std::string match) {
	Token t = getNext();
	if (t.type != match) {
		std::cout << t.value << " " << t.type << std::endl;
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
	AstNode programNode(program);
	while (!isEmpty()) {
		AstNode functionNode = parseFunction();
		programNode.addNodeFront(functionNode);
	}
	ast = std::make_shared<AstNode>(programNode);
}

AstNode Parser::parseFunction() {
	AstNode functionNode(function);
	abortMatch("int");
	Token t = getNext();
	if (t.type != "identifier") {
		abort("identifier");
	}
	functionNode.addValue(t.value);
	abortMatch("(");
	int numberOfParameters = 0;
	while(contains(typeDict, peekNext())) {
		getNext();
		Token ident = getNext();
		AstNode param(parameterFunctionPrototype, ident.value);
		functionNode.addNode(param);
		if (!match(",", peekNext())) {
			break;
		}
		else {
			getNext();
		}
	}
	abortMatch(")");
	abortMatch("{");
	AstNode body = parseBody();
	if (!body.scopeFlag) {
		AstNode statementBlock(irrelevant);
		AstNode ret(returnStatement, "return");
		statementBlock.addNode(ret);
		statementBlock.addNode(AstNode(constant, "0"));
		body.addNodeFront(statementBlock);
	} else {
		functionNode.addNodeFront(body);
	}
	abortMatch("}");
//	function.addNode("numberParameters", std::to_string(numberOfParameters));
	return function;
}

AstNode Parser::parseBody() {
	AstNode body(irrelevant);
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
	AstNode numDecls(numberDeclarations, std::to_string(declarations));
	body.addNode(numDecls);
	body.scopeFlag = returnInScope;
	return body;
}

AstNode Parser::parseWhileStatement() {
	AstNode whileBlock(whileBlock);
	AstNode ifBlock(ifBlock);
	AstNode ifStatement(irrelevant);
	Token t = getNext();
	abortMatch("(");
	AstNode condition = parseExpression();
	abortMatch(")");
	abortMatch("{");
	AstNode ifBody = parseBody();
	ifBody.addType(whileBody);
	abortMatch("}");
	ifBlock.addNode(AstNode(whileElseBody));
	ifBlock.addNode(AstNode(whileLoop));
	ifBlock.addNode(ifBody);
	ifStatement.addNode(ifBlock);
	ifStatement.addNode(condition);
	whileBlock.addNode(ifStatement);
	return whileBlock;
}

AstNode Parser::parseIfStatement() {
	AstNode ifBlockNode(ifBlock);
	AstNode ifStatement(irrelevant);
	Token t = getNext();
	abortMatch("(");
	AstNode condition = parseExpression();
	abortMatch(")");
	abortMatch("{");
	AstNode ifBodyNode = parseBody();
	ifBodyNode.addType(ifBody);
	abortMatch("}");
	if (match("else", peekNext())) {
		Token t = getNext();
		abortMatch("{");
		AstNode elseBodyNode = parseBody();
		elseBodyNode.addType(elseBody);
		ifBlockNode.addNode(elseBodyNode);
		abortMatch("}");
	}
	else {
		AstNode elseBodyNode(elseBody);
		ifBlockNode.addNode(elseBodyNode);
	}
	ifBlockNode.addNode(ifBodyNode);
	ifStatement.addNode(ifBlock);
	ifStatement.addNode(condition);
	return ifStatement;
}

AstNode Parser::parseDecl() {
	AstNode decl(irrelevant);
	Token t = getNext();
	t = getNext();
	if (!match("identifier", t.type)) {
		abort("identifier");
	}
	AstNode variable(declaration, t.value);
	if (match("=", peekNext())) {
		AstNode assign = parseAssign(t.value);
		decl.addNode(assign);
	}
	else {
		abortMatch(";");
	}
	decl.addNode(variable);
	return decl;
}

AstNode Parser::parseAssign(std::string value) {
	AstNode assign(irrelevant);
	Token t = getNext(); //incrementing by one token to pass assingment operator
	AstNode assignVar(identifierAssign, value);
	AstNode expr = parseExpression(); //getting expression to parse with
	assign.addNode(assignVar);
	assign.addNode(expr);//since in post order, want to identify expression first when generating assembly
	abortMatch(";");
	return assign;
}

AstNode Parser::parseReturnStatement() {
	AstNode statementBlock(irrelevant);
	Token t = getNext();
	if (!contains(statementDict, t.type)) {
		abort("statement");
	}
	AstNode exp = parseExpression();
	AstNode statement(returnStatement, t.type);
	statementBlock.addNode(statement);
	statementBlock.addNode(exp);
	abortMatch(";");
	return statementBlock;
}

AstNode Parser::parseExpression() {
	AstNode expr(irrelevant);
	AstNode boolTerm = parseAnd();
	if (!match("||", peekNext())) return boolTerm;
	while (match("||", peekNext())) {
		AstNode op(boolOp, getNext().type);
		AstNode nextTerm = parseAnd();
		op.addNode(nextTerm);
		expr.addNode(op);
	}
	expr.addNode(boolTerm);
	return expr;
}

AstNode Parser::parseAnd() {
	AstNode boolTerm(irrelevant);
	AstNode equalityTerm = parseEquality();
	if (!match("&&", peekNext())) return equalityTerm;
	while (match("&&", peekNext())) {
		AstNode op(boolOp, getNext().type);
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
	AstNode equalityTerm(irrelevant);
	AstNode relTerm = parseRelational();
	if (!contains(equalityOpDict, peekNext())) return relTerm;
	while (contains(equalityOpDict, peekNext())) {
		AstNode op(equalityOp, getNext().type);
		AstNode nextTerm = parseRelational();
		op.addNode(nextTerm);
		equalityTerm.addNode(op);
	}
	equalityTerm.addNode(relTerm);
	return equalityTerm;
}

AstNode Parser::parseRelational() {
	AstNode relationalTerm(irrelevant);
	AstNode arith = parseArithmetic();
	if (!contains(relOpDict, peekNext())) return arith;
	while (contains(relOpDict, peekNext())) {
		AstNode op(relationOp, getNext().type); 
		AstNode nextArith = parseArithmetic();
		op.addNode(nextArith);
		relationalTerm.addNode(op);
	}
	relationalTerm.addNode(arith);
	return relationalTerm;
}

AstNode Parser::parseArithmetic() {
	AstNode expr(irrelevant);
	AstNode term = parseTerm();
	if (!contains(addOpDict, peekNext())) return term;
	while (contains(addOpDict, peekNext())) {
		AstNode op(addOp, getNext().type);
		AstNode nextTerm = parseTerm();
		op.addNode(nextTerm);
		expr.addNode(op);
	}
	expr.addNode(term);
	return expr;
}

AstNode Parser::parseTerm() {
	AstNode term(irrelevant);
	AstNode factor = parseFactor();
	if (!contains(mulOpDict, peekNext())) return factor;
	while (contains(mulOpDict, peekNext())) {
		AstNode op(multiplyOp, getNext().type);
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
		return AstNode(constant,t.value);
	}
	else if (match("identifier", t.type)) {
		if (match("(", peekNext())) {
			return parseFuncTerm(t.value);
		}
		return AstNode(identifierAccess, t.value);
	}
	else if (contains(unaryDict, t.type)) {
		AstNode expr = parseFactor();
		return UnOp(t.type, expr);
	}
	abort("expression");
}

AstNode Parser::parseFuncTerm(std::string functionName) {
	abortMatch("(");
	AstNode funcTerm(funcTerm);
	funcTerm.addValue(functionName);
	while (match("identifier", peekNext()) || match("number", peekNext())) {
		AstNode param(parameterFunctionCalling, parseExpression());
		funcTerm.addNodeFront(param);
		if (!match(",", peekNext())) {
			break;
		}
		else {
			getNext();
		}
	}
	abortMatch(")");
	return funcTerm;
}

AstNode Parser::UnOp(std::string type, AstNode expr) {
	expr.addNode(AstNode(unaryOp, type));
	return expr;
}

void Parser::print() {
	printHelper(ast);
}

void Parser::printHelper(std::shared_ptr<AstNode> &node) {
	if (!node) return;
	std::cout << node->value << std::endl;
	for (auto &child : node->children) {
		printHelper(child);
	}
}
