#ifndef ASTNODE_H
#define ASTNODE_H
#include <string>
#include <iostream>
#include <vector>
#include <deque>
#include <memory>

enum AstType {
	program,
	function,
	funcTerm,
	parameterFunctionPrototype,
	parameterFunctionCalling,
	constant,
	returnStatement,
	identifierAssign,
	identifierAccess,
	numberDeclarations,
	declaration,
	whileBlock,
	whileBody,
	whileElseBody,
	whileLoop,
	ifBlock,
	ifBody,
	elseBody,
	boolOp,
	equalityOp,
	relationOp,
	addOp,
	multiplyOp,
	unaryOp,
	irrelevant
};

class AstNode {
	public:
		AstNode();
		AstNode(AstType _type);
		AstNode(AstType _type, std::string _value);
		AstNode(AstType _type, AstNode child);
		AstNode(AstType _type, std::string _value, AstNode child);
		void addType(AstType _type);
		void addValue(std::string _value);
		void addNode(AstNode node);
		void addNodeFront(AstNode node);
		void clearNodes();
		AstType type;
		std::string value;
		bool scopeFlag; //Scope flag is used only by function nodes to determine if a return statement was declared in the function body
		std::deque< std::shared_ptr<AstNode> > children;
};
#endif
