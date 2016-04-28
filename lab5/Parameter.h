#pragma once

#include <string>

using namespace std;

class Parameter
{
private:
	string para;
	string type;

public:

	Parameter()
	{

	}
	void setPara(string temp_para, string temp_type)
	{
		para = temp_para;
		type = temp_type;
	}

	string toSting()
	{
		if(type == "STRING")
		{
			return "\'" + para + "\'";
		}
	
		return para;
	
	}

};