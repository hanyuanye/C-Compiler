#ifndef ASTNODE_H
#define ASTNODE_H
#include <string>
#include <iostream>
#include <vector>
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
		void clearNodes();
		std::string type;
		std::string value;
		std::vector< std::shared_ptr<AstNode> > children;
};
#endif
