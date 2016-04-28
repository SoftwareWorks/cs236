#pragma once

#include <string>
#include <vector>
#include <set>
#include "Predicate.h"
#include "Parameter.h"
#include "Rule.h"
#include "Parser.h"
#include "scanner.h"
#include "Fact.h"
#include "Query.h"
#include "Scheme.h"


using namespace std;

class Datalog
{
private:
	
	string output;
	Scheme sc;
	Fact fa;
	Query qu;
	vector<Rule> ru;

	set<string> domain;

public:

	Datalog(Scheme temp1, Fact temp2, Query temp3, set<string> temp4, vector<Rule> temp5)
	{
		sc = temp1;
		fa = temp2;
		qu = temp3;
		domain = temp4;
		ru = temp5;
	}

	string printdomain()
	{
		string strings = "";
		set<string>::iterator iter;

		for(iter=domain.begin(); iter!=domain.end(); ++iter)
		{
			strings = strings + "\'" + *iter + "\'\n";
		}


		return strings;
	}

	string printrule()
	{
		string rules = "";

		for(int i = 0; i < ru.size(); i++)
		{
			rules = rules + ru[i].toString();
		}

		return rules;
	}

	string toString()		// create a string that holds all the information
	{
		string sc_size = int_to_string(sc.scheme_size());
		string fa_size = int_to_string(fa.fact_size());
		string qu_size = int_to_string(qu.query_size());
		string do_size = int_to_string(domain.size());
		string ru_size = int_to_string(ru.size());

		output = "Success!\n";
		output = output + "Schemes(" + sc_size + "):\n";
		output = output + sc.toString();
		output = output + "Facts(" + fa_size + "):\n";
		output = output + fa.toString();
		output = output + "Rules(" + ru_size + "):\n"; 
		output = output + printrule();
		output = output + "Queries(" + qu_size + "):\n";
		output = output + qu.toString();
		output = output + "Domain(" + do_size + "):\n";
		output = output + printdomain();
		

		return output;
	}

	string int_to_string(int number)
	{
		stringstream ss;
		ss << number;
		return ss.str();
	}

};