#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <set>
#include "scanner.h"
#include "token.h"
#include "Datalog.h"
#include "Parameter.h"
#include "Predicate.h"
#include "Rule.h"

using namespace std;

class Parser
{
private:
	vector<token> vec;
	int index;
	
	set<string> domain;
	vector<Predicate> schemelist;
	vector<Predicate> factlist;
	vector<Predicate> querylist;
	vector<Predicate> predlist;
	vector<Rule> rulelist;

public:
	Parser(vector<token> temp)
	{
		vec = temp;
		index = 0;
	}

	void check_Datalog()
	{

		match("SCHEMES");
		match("COLON");
		if(vec[index].getType() == "FACTS")
		{
			error();
		}
		check_Schemelist(vec[index].getType());
		match("FACTS");
		match("COLON");
		check_Factlist(vec[index].getType());
		match("RULES");
		match("COLON");
		check_Rulelist(vec[index].getType());
		match("QUERIES");
		match("COLON");
		check_Querylist(vec[index].getType());
		
		// check if all the elements in the vec is gone through

	}

	void check_Schemelist(string type)		// type is a variable for token type to be checked
	{
		if(type == "FACTS")
		{
			return;
		}
		else
		{
			check_Scheme(type);

			check_Schemelist(vec[index].getType());
		}
	}

	void check_Scheme(string type)
	{
		schemelist.push_back(check_Predicate(type));
	}

	void check_Factlist(string type)
	{

		if(type == "RULES")
		{
			return;
		}
		else
		{
			check_Fact(type);
			check_Factlist(vec[index].getType());
		}
	}

	void check_Fact(string type)
	{
		factlist.push_back(check_Predicate(type));
		match("PERIOD");

	}

	void check_Rulelist(string type)
	{

		if(type == "QUERIES")
		{
			return;
		}
		else
		{
			check_Rule(type);
			check_Rulelist(vec[index].getType());
		}
	}

	void check_Rule(string type)
	{
		Rule rule;

		rule.setHead(check_Predicate(type));
		match("COLON_DASH");
		check_Predicatelist(vec[index].getType(), rule);
		match("PERIOD");

		rulelist.push_back(rule);
	}

	void check_Querylist(string type)
	{

		check_Query(type);
		if(index >= vec.size())
		{
			return;
		}
		else
		{
			check_Querylist(vec[index].getType());
		}

	}
	void check_Query(string type)
	{

		querylist.push_back(check_Predicate(type));
		match("Q_MARK");
	}

	void check_Predicatelist(string type, Rule& rule)
	{
		rule.add_pred_list(check_Predicate(type));
		if(vec[index].getType() == "COMMA")
		{
			match("COMMA");
			check_Predicatelist(vec[index].getType(), rule);
		}
		else
		{
			return;
		}
		
	}


	Predicate check_Predicate(string type)
	{
		
		Predicate pred;

		pred.setHead(vec[index].getValue());
		match("ID");
		match("LEFT_PAREN");
		if(vec[index].getType() == "RIGHT_PAREN")
		{
			error();
		}
		check_Parameterlist(vec[index].getType(), pred);
		match("RIGHT_PAREN");
		


		return pred;
	}

	void check_Parameterlist(string type, Predicate& pred)
	{

		if(type == "RIGHT_PAREN")
		{
			return;
		}
		else
		{
			
			check_Parameter(type, pred);
			if(vec[index].getType() == "COMMA")
			{
				match("COMMA");
				if(vec[index].getType() == "RIGHT_PAREN")
				{
					error();
				}
				check_Parameterlist(vec[index].getType(), pred);
			}
			else
			{
				return;
			}
		}
	}

	void check_Parameter(string type, Predicate& pred)
	{
		Parameter para;
		if(type == "STRING")
		{
			domain.insert(vec[index].getValue());
			para.setPara(vec[index].getValue(),"STRING");
			pred.add_para_list(para);
			match("STRING");
			return;
		}
		else if(type == "ID")
		{
			para.setPara(vec[index].getValue(),"ID");
			pred.add_para_list(para);
			match("ID");
			return;
		}
		else		// error
		{
			error();
		}
	}
	
	void match(string type)
	{
		if(vec[index].getType() == type)
		{
			if(index >= vec.size())		// make sure that there is something in Query List
			{
				error();
			}
			else
			{
				index ++;
			}
			
		}
		else		// error
		{
			error();
		}
	}

	void error()
	{
		string result = "  (" + vec[index].getType() + ",\"" + vec[index].getValue() + "\"," + int_to_string(vec[index].getline()) + ")"; // add line number
		throw result;
	}

	string int_to_string(int number)
	{
		stringstream ss;
		ss << number;
		return ss.str();
	}

	vector<Predicate> accessSchemelist()
	{
		return schemelist;
	}
	vector<Predicate> accessFactlist()
	{
		return factlist;
	}	
	vector<Predicate> accessQuerylist()
	{
		return querylist;
	}
	vector<Rule> accessRulelist()
	{
		return rulelist;
	}
	set<string> accessDomain()
	{
		return domain;
	}
};