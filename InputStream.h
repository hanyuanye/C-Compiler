#include <iostream>
#include <cstdlib>
class InputStream {
public:
	InputStream(std::string s);	
	void getChar();
	void error(std::string s);
	void abort(std::string s);
	void expect(std::string s);
	void match(char c);
	bool isAlpha(char c);
	bool isDigit(char c);
	char getName();
	char getNum();
private:
	char look;
	std::string code;
	int index;
	
};
