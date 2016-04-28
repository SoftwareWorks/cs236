#pragma once

#include <string>
#include <vector>
#include "Predicate.h"

using namespace std;

class Fact
{
private:
	string output;
	vector<Predicate> pred_list;

public:
	
	Fact()
	{
	}
	Fact(vector<Predicate> temp)
	{
		pred_list = temp;
	}

	vector<Predicate> getFactlist()
	{
		return pred_list;
	}

	int fact_size()
	{
		return pred_list.size();
	}

	string toString()
	{
		output = "";
		for(int i = 0; i < pred_list.size(); i++)
		{
			output = output + "  " + pred_list[i].toString() + ".\n";
		}
		return output;
	}

};