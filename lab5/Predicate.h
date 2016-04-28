#pragma once

#include <string>
#include <vector>
#include "Parameter.h"

using namespace std;

class Predicate
{
private:
	string head;
	vector<Parameter> paralist;
	string output;

public:
	Predicate()
	{
	}

	string getHead()
	{
		return head;
	}
	vector<string> getParalist()
	{
		vector<string> parainstring;
		for(int i = 0; i < paralist.size(); i++)
		{
			parainstring.push_back(paralist[i].toSting());
		}

		return parainstring;
	}
	void setHead(string temp)
	{
		head = temp;
	}
	void add_para_list(Parameter temp)
	{
		paralist.push_back(temp);
	}

	int para_size()
	{
		return paralist.size();
	}

	string toString()
	{
		output = head + "(";
		for(int i = 0; i < paralist.size(); i++)
		{
			string temp = "";
			if(i == paralist.size()-1)
			{
				temp = paralist[i].toSting();
			}
			else
			{
				temp = paralist[i].toSting() + ",";
			}
			output = output + temp;
		}
		output = output + ")";

		return output;
	}

};