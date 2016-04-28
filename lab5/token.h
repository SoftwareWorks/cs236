#pragma once

#include <string>

using namespace std;

class token
{
private:
	string type;
	string value;
	int line;
	
public:

	token(int error_line)
	{
		line = error_line;
	}

	token(string token_type, string token_value, int token_line)
	{
		type = token_type;
		value = token_value;
		line = token_line;

	}

	string getType()
	{
		return type;
	}
	string getValue()
	{
		return value;
	}	
	int getline()
	{
		return line;
	}
	int geterrorline()
	{
		return line;
	}
};