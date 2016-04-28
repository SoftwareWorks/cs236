#pragma once

#include <string>
#include <vector>
#include "Predicate.h"

using namespace std;

class Query
{
private:
	string output;
	vector<Predicate> pred_list;

public:
	
	Query()
	{
	}
	Query(vector<Predicate> temp)
	{
		pred_list = temp;
	}

	vector<Predicate> getQuerylist()
	{
		return pred_list;
	}

	int query_size()
	{
		return pred_list.size();
	}

	string toString()
	{
		output = "";
		for(int i = 0; i < pred_list.size(); i++)
		{
			output = output + "  " + pred_list[i].toString() + "?\n";
		}
		return output;
	}

};