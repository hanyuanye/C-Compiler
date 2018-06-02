run:		lexer
		./compiler

compiler:	InputStream.h InputStream.cpp compiler.cpp Parser.h Parser.cpp 
		g++ InputStream.cpp Parser.cpp compiler.cpp -o compiler

lexer:		generateAST.cpp generateAST.h lexer.cpp lexer.h compiler.cpp
		g++ -std=c++11 lexer.cpp  generateAST.cpp compiler.cpp -o compiler

clean:		rm -f compiler
