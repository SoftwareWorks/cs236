#pragma once

#include <sstream>
#include <string>
#include <set>
#include <vector>
#include "Relation.h"
#include "Query.h"
#include "DBmanager.h"
#include "Fact.h"
#include "Scheme.h"

using namespace std;

class Database
{
		
private:
	vector<Relation> relations;
	Query qr;
	DBmanager db;
	vector<Predicate> querylist;
	vector<string> querypara;
	Relation ra;
	string output;
	Predicate query;
	vector<vector <string> > projected;
	vector<int> nonterminal;

public:
	Database(Query temp1, DBmanager temp2)		// this class is to 
	{
		output = "";
		qr = temp1;
		querylist = qr.getQuerylist();
		cout << "size of querylist " << querylist.size() << endl;
		db = temp2;
		relations = db.getRelationlist();
		cout << "size of relationlist " << relations.size() << endl << endl;
	}

	void reconstruct()
	{
		for(int i = 0; i < qr.query_size(); i++)
		{
			for(int j = 0; j < relations.size(); j++)
			{
				if(querylist[i].getHead() == relations[j].getName())		// to find a relation that matches the query
				{
					ra = queryAttribute(i, j);	 // after this line, selection, project, and rename are done

					toString1(ra, querylist[i]);

					cout << "\nend of one query========" << endl << endl;
				}
			}
		}
	}

	string toString1(Relation temp_relation, Predicate temp_query)
	{
		output = output + temp_query.getHead() + "(";
		for(int i = 0; i < temp_query.para_size(); i++)
		{
			if(i == temp_query.para_size()-1)
			{
				output = output + temp_query.getParalist().at(i) + ")";
			}
			else
			{
				output = output + temp_query.getParalist().at(i) + ",";
			}
		}
		output = output + "? ";
		
		toString2(temp_relation, temp_query);

		return output;
	}
	void toString2(Relation &temp_relation, Predicate &temp_query)
	{	
		if(temp_relation.tuplesize() > 0)
		{
			output = output + "Yes(" + int_to_string(temp_relation.tuplesize()) + ")\n";

			cout << "size of projected ? " << projected.size() << endl;
			

			for(int j = 0; j < temp_relation.tuplesize(); j++)
			{
				if(projected.size() != 0)
				{
					output = output + "  ";
				}

				toString3(temp_relation, temp_query, j);

				if(projected.size() != 0)
				{
					output = output + "\n";
				}
			}
		}
		else
		{
			output = output + "No\n";
		}
	}
	void toString3(Relation &temp_relation, Predicate &temp_query, int j)
	{

		for(int i = 0; i < projected.size(); i++)
		{
			
			cout << nonterminal[i] << endl;

			output = output + temp_relation.getSchema().getSchema().at(nonterminal[i]) + "=";

			output = output + projected[i].at(j);

			if(i != projected.size()-1)
			{
				output = output + ", ";
			}
		}

	}

	string int_to_string(int number)
	{
		stringstream ss;
		ss << number;
		return ss.str();
	}

	string toMain()
	{
		return output;
	}

	Relation queryAttribute(int i, int j)
	{			
		
		Relation result = relations[j];
		querypara = querylist[i].getParalist();
		vector<int> nonterminals;		// vector of nonterminals position
		vector<int> duplicates;			// vector of duplicates position

		for(int k = 0; k < querypara.size(); k++)		// after this for loop, selection is done
		{
			if(querypara[k][0] == '\'')		// select1 operation, find a Terminal attribute in the query and select tuples accordingly
			{
				Relation r1;
				r1 = result.select1(k, querypara[k]);
				result = r1;
			}
			else			// for non-Terminal attirubute in the query and check for duplicate values as well
			{
				nonterminals.push_back(k);
				for(int l = k+1; l < querypara.size(); l++)
				{
					if(querypara[k] == querypara[l])
					{
						duplicates.push_back(l);
						Relation r1;
						r1 = result.select2(k, l);
						result = r1;
					}
				}
			}
		}

		cout << "select " << endl;
		cout << "name of the query " << result.getName() << endl;
		cout << "attributes in the query ";
		for(int q = 0; q < querypara.size(); q++)
		{
			cout << querypara.at(q) << " ";
		}
		cout << "\nsize of the selected tuples " << result.tuplesize() << endl;

		cout << "position of nonterminal " << endl;
		for(int w = 0; w < nonterminals.size(); w++)
		{
			cout << nonterminals[w] << " ";
		}
		cout << "\nposition of duplicate " << endl;
		for(int y = 0; y < duplicates.size(); y++)
		{
			cout << duplicates[y] << " ";
		}
		cout << endl;

		result = forproject1(nonterminals, duplicates, result);		// calls a function for project and rename

		return result;
	}
	void forproject2(vector<int> &nonterminals, vector<int> &duplicates, Relation &result, int &i)
	{
		result = result.project(nonterminals[i]);
		if(result.getProjected().size() > 0)
		{
			cout << "after projected, in the vector " << result.getProjected().at(0) << endl;
		}
		projected.push_back(result.getProjected());

		result = result.rename(nonterminals[i], querypara[nonterminals[i]]);
		cout << "what is renamed value " << result.getSchema().getSchema().at(0) << endl;

	}
	Relation forproject1(vector<int> nonterminals, vector<int> duplicates, Relation result)
	{
		clearProjected();
		if(duplicates.size() > 0)
		{

			for(int y = 0; y < duplicates.size(); y++)
			{

				for(int i = 0; i < nonterminals.size(); i++)
				{
					if(duplicates[y] == nonterminals[i])
					{			
						nonterminals.erase(nonterminals.begin()+i);
					}
				}
			}

			for(int i = 0; i < nonterminals.size(); i++)
			{
				forproject2(nonterminals, duplicates, result, i);
			}
			//for(int y = 0; y < duplicates.size(); y++)
			//{
			//	// 여기서 뭘 수정해서 벡터에 duplicate 벨류가 안들어가게 고쳐야됨

			//	for(int i = 0; i < nonterminals.size(); i++)
			//	{
			//		if(duplicates[y] != nonterminals[i])
			//		{
			//			forproject2(nonterminals, duplicates, result, i);
			//		}
			//	}

			//}
		}
		else
		{
			for(int i = 0; i < nonterminals.size(); i++)
			{
				forproject2(nonterminals, duplicates, result, i);
			}
		}

		setNonterminals(nonterminals);

		return result;

	}
	void clearProjected()
	{
		while(!projected.empty())
		{
			projected.pop_back();
		}
	}
	void setNonterminals(vector<int> nonterminals)
	{
		nonterminal = nonterminals;
	}
};
