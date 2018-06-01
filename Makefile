run:		compiler
		./compiler

compiler:	InputStream.h InputStream.cpp compiler.cpp 
		g++ InputStream.cpp compiler.cpp -o compiler

clean:		rm -f compiler
