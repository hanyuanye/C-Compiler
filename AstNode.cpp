#include "AstNode.h"

AstNode::AstNode() {}

AstNode::AstNode(std::string _type, std::string _value) {
	type = _type;
	value = _value;
}

AstNode::AstNode(std::string _type, AstNode child) {
	type = _type;
	children.emplace_back(new AstNode(child));
}
