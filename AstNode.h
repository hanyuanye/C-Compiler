#ifndef ASTNODE_H
#define ASTNODE_H
#include <string>
#include <iostream>
#include <vector>
#include <deque>
#include <memory>
class AstNode {
	public:
		AstNode();
		AstNode(std::string _type);
		AstNode(std::string _type, std::string _value);
		AstNode(std::string _type, AstNode child);
		AstNode(std::string _type, std::string _value, AstNode child);
		void addType(std::string _type);
		void addValue(std::string _value);
		void addNode(AstNode node);
		void addNodeFront(AstNode node);
		void clearNodes();
		std::string type;
		std::string value;
		bool scopeFlag; //Scope flag is used only by function nodes to determine if a return statement was declared in the function body
		std::deque< std::shared_ptr<AstNode> > children;
};
#endif
