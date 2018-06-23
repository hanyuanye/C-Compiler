#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <deque>
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
		std::string getOffset(std::string identifier);
		std::vector< std::unordered_map<std::string, int> > variableMap;
		std::unordered_set<std::string> functionMap;
		bool addFunction(std::shared_ptr<AstNode> ast);

		std::list<int> increments;
		std::deque<int> branchStack;
		std::string code;

		void push(std::string value, int increment);
		void pop(std::string value);
		int varStackIndex;
		int paramIndex;
		int currStack;

		std::string pushBranch();
		std::string popBranch();
		std::string popSecondBranch();
		int branchCount;

		void pushScope();
		void popScope();
		int scopeCount;
};

