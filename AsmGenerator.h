#include <iostream>
#include <fstream>
#include "AstNode.h"

class AsmGenerator {
	public:
		void emit();
		void generateAssembly(std::shared_ptr<AstNode> ast);
	private:
		void generateAsmChildren(std::shared_ptr<AstNode> ast);
		void emitln(std::string s);
		std::string code;
};
