
#include "Parser.h"

const std::string statementArr[] = {"return"};
const std::unordered_set<std::string> Parser::statementDict(statementArr, statementArr + sizeof(statementArr)/sizeof(statementArr[0]));

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
};

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
	Token t = getNext();
	if (t.type != "int") {
		abort("type");
	}
	t = getNext();
	if (t.type != "identifier") {
		abort("identifier");
	}
	function.addValue(t.value);
	t = getNext();
	if (t.type != "(") {
		abort("(");
	}
	t = getNext();
	if (t.type != ")") {
		abort(")");
	}
	t = getNext();
	if (t.type != "{") {
		abort("{");
	}
	AstNode body = parseBody();
	t = getNext();
	if (t.type != "}") {
		abort ("}");
	}
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
		abort("statement");
	} 	
	AstNode exp = parseExpression();
	AstNode statement("statement", exp);	
	t = getNext();
	if (t.type != ";") {
		abort("semicolon");
	}
	return statement;
}	

AstNode Parser::parseExpression() {
	Token t = getNext();
	if (t.type != "idenfitifier" && t.type != "number") {
		abort("value");
	}
	std::string value;
	if (t.type == "identifier") {
		value = t.type;
	} 
	else if (t.type == "number") {
		value = t.value;
	}
	return AstNode("const", value);
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