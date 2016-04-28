#pragma once

#include <string>

using namespace std;

class Rule
{
private:
	Predicate head;
	vector<Predicate> pred_list;

public:
	Rule()
	{

	}

	//Rule(vector<Predicate> temp)
	//{
	//	pred_list = temp;
	//}

	void setHead(Predicate temp)
	{
		head = temp;
	}
	void add_pred_list(Predicate temp)
	{
		pred_list.push_back(temp);
	}

	vector<Predicate> getBodylist()
	{
		return pred_list;
	}
	Predicate getRulehead()
	{
		return head;
	}

	string toString()
	{
		string tails = "";
		string output = "";
		for(int i = 0; i < pred_list.size(); i++)
		{
			if(i == pred_list.size()-1)
			{
				tails = tails + pred_list[i].toString();
			}
			else
			{
				tails = tails + pred_list[i].toString() + ",";
			}
		}
		output = "  " + head.toString() + " :- " + tails + "\n";
		return output;
	}
};