#include "Parser.h"

const std::string statementArr[] = {"return"};
const std::unordered_set<std::string> Parser::statementDict(statementArr, statementArr + sizeof(statementArr)/sizeof(statementArr[0]));
const std::string operatorsArr[] = {"+","-","*","/"};
const std::unordered_set<std::string> Parser::operatorDict(operatorsArr, operatorsArr + sizeof(operatorsArr)/sizeof(operatorsArr[0]));
const std::string unaryArr[] = {"-","~","!"};
const std::unordered_set<std::string> Parser::unaryDict(unaryArr, unaryArr + sizeof(unaryArr)/sizeof(unaryArr[0]));

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

void Parser::abortMatch(std::string match) {
	Token t = getNext();
	if (t.type != match) {
		abort(match);
	}
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
	abortMatch("}");
	function.addNode(body);
	return function;
}

AstNode Parser::parseBody() {
	AstNode body("body");
	AstNode statement = parseStatement();
	auto v = statement.children;
	statement.clearNodes();
	body.addNode(statement);
	for (auto& node : v) {
		body.addNode(*node);
	}
	return body;
}

AstNode Parser::parseStatement() {
	Token t = getNext();
	if (!contains(statementDict, t.type)) {
		abortMatch("statement");
	} 	
	AstNode exp = parseExpression();
	AstNode statement("statement", exp);
	abortMatch(";");	
	return statement;
}	

AstNode Parser::parseExpression() {
	AstNode("const");
	Token t = getNext();
	if (!contains(unaryDict, t.type) && t.type != "identifier" && t.type != "number") {
		abortMatch("expression");
	}
	std::string value;
	if (t.type == "identifier") {
		value = t.type;
		return AstNode("const", value);
	} 
	else if (t.type == "number") {
		value = t.value;
		return AstNode("const", value);
	}
	else if (contains(unaryDict, t.type)) {
		AstNode expr = parseExpression();
		return UnOp(t.type, expr);
	}
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
