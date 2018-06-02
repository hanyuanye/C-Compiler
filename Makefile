run:		lexer
		./compiler

compiler:	InputStream.h InputStream.cpp compiler.cpp Parser.h Parser.cpp 
		g++ InputStream.cpp Parser.cpp compiler.cpp -o compiler

lexer:		lexer.cpp lexer.h compiler.cpp
		g++ -std=c++11 lexer.cpp  compiler.cpp -o compiler

clean:		rm -f compiler
