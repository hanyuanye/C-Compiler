#include <fstream>
#include <string>
#include <iostream>
#include "InputStream.h"
#include "Parser.h"

int main(int argc, char** argv)
{
	std::string s = "K + 4 * 3";
	InputStream is(s);
	Parser parser(is);
	parser.expression();
}
