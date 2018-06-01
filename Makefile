run:		compiler
		./compiler

compiler:	InputStream.h InputStream.cpp compiler.cpp Parser.h Parser.cpp 
		g++ InputStream.cpp Parser.cpp compiler.cpp -o compiler

clean:		rm -f compiler
