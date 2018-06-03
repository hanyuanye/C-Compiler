#ifndef ASTNODE_H
#define ASTNODE_H
#include <string>
#include <iostream>
#include <vector>
#include <memory>
class AstNode {
	public:
		AstNode();
		AstNode(std::string _type, std::string _value);
		AstNode(std::string _type, AstNode child);
		std::string type;
		std::string value;
		std::vector< std::shared_ptr<AstNode> > children;
};
#endif
