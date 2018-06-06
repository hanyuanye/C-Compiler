run:		compiler
		./compiler

compiler:	AsmGenerator.cpp AsmGenerator.h AstNode.cpp AstNode.h Parser.cpp Parser.h lexer.cpp lexer.h compiler.cpp
		g++ -std=c++14 -g AsmGenerator.cpp AstNode.cpp lexer.cpp  Parser.cpp compiler.cpp -o compiler

compile:		
		./compiler

asm:		assembly
		./out; echo $$?

assembly:	assembly.s
		gcc -m32 assembly.s -o out

clean:		
		rm -f compiler out
