#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
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
		std::vector< std::unordered_map<std::string, int> > variableMap;
		int stackIndex;
		int scopeCount;
		std::string code;
};

