run:		lexer
		./compiler

compiler:	InputStream.h InputStream.cpp compiler.cpp Parser.h Parser.cpp 
		g++ InputStream.cpp Parser.cpp compiler.cpp -o compiler

lexer:		AsmGenerator.cpp AsmGenerator.h AstNode.cpp AstNode.h Parser.cpp Parser.h lexer.cpp lexer.h compiler.cpp
		g++ -std=c++14 -g AsmGenerator.cpp AstNode.cpp lexer.cpp  Parser.cpp compiler.cpp -o compiler

asm:		assembly
		./out; echo $$?

assembly:	assembly.s
		gcc -m32 assembly.s -o out

clean:		
		rm -f compiler out
