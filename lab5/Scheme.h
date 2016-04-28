#pragma once

#include <string>
#include <vector>
#include "Predicate.h"

using namespace std;

class Scheme
{
private:
	vector<Predicate> pred_list;
	string output;

public:
	
	Scheme()
	{
	}
	Scheme(vector<Predicate> temp)
	{
		pred_list = temp;
	}


	vector<Predicate> getSchemelist()
	{
		return pred_list;
	}
	int scheme_size()
	{
		return pred_list.size();
	}
	string toString()
	{
		output = "";
		for(int i = 0; i < pred_list.size(); i++)
		{
			output = output + "  " + pred_list[i].toString() + "\n";
		}
		return output;
	}

};