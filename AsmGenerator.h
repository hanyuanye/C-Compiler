#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <list>
#include <map>
#include <utility>
#include "AstNode.h"

class AsmGenerator {
	public:
		AsmGenerator();
		void emit();
		void generateAssembly(std::shared_ptr<AstNode> ast);
	private:
		void generateAsmChildren(std::shared_ptr<AstNode> ast);
		void emitln(std::string s);
		void error(std::string msg);
		void push(std::string value, int increment);
		void pop(std::string value);
		std::string getOffset(std::string identifier);
		std::vector< std::unordered_map<std::string, int> > variableMap;
		int varStackIndex;
		int currStack;
		std::list<int> increments;
		int scopeCount;
		std::string code;
};

