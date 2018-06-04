#include "AstNode.h"

AstNode::AstNode() {}

AstNode::AstNode(std::string _type) {
	type = _type;
}

AstNode::AstNode(std::string _type, std::string _value) {
	type = _type;
	value = _value;
}

AstNode::AstNode(std::string _type, AstNode child) {
	type = _type;
	children.emplace_back(new AstNode(child));
}

AstNode::AstNode(std::string _type, std::string _value, AstNode child) {
	type = _type;
	value = _value;
	children.emplace_back(new AstNode(child));
}

void AstNode::addValue(std::string _value) {
	value = _value;
}

void AstNode::addNode(AstNode node) {
	children.emplace_back(new AstNode(node));
}

void AstNode::clearNodes() {
	children.clear();
}
