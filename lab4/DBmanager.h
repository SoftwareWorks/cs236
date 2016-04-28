#pragma once

#include <string>
#include <vector>
#include "Scheme.h"
#include "Fact.h"
#include "Query.h"
#include "Rule.h"
#include "Relation.h"
#include <iostream>
#include "Tuple.h"

class DBmanager			// this class creates Database
{
private:
	vector<Predicate> schemelist;
	vector<Predicate> factlist;
	vector<Predicate> querylist;
	vector<Rule> rulelist;
	Scheme sc;
	Fact fa;
	Rule ru;
	vector<Relation> relationlist;
	vector<string> bodypara;
	vector<int> nonterminal;
	vector<vector <string> > projected;
	int counter;



public:

	DBmanager()
	{
	}
	DBmanager(Scheme temp1, Fact &temp2, vector<Rule> temp3)
	{
		counter = 0;
		sc = temp1;
		fa = temp2;
		schemelist = sc.getSchemelist();
		factlist = fa.getFactlist();
		rulelist = temp3;
	}

	int getCounter()
	{
		return counter;
	}
	vector<Relation> getRelationlist()
	{
		return relationlist;
	}
	void makeRelation()
	{

		for(int i = 0; i < schemelist.size(); i++)	  // creating a relation object for each loop
		{
			Relation ra;
			Schema sm;

			ra.setName(schemelist[i].getHead());		// set name for a relation from the schemelist

			//			cout << schemelist[i].getHead() << endl;



			sm.setSchema(schemelist[i].getParalist());		// set a schema for a relation from the schemelist

			ra.setSchema(sm);

			//			cout << sm.getSchema().at(0) << endl;

			for(int j = 0; j < factlist.size(); j++)		// extract tuples for a relation from the factlist
			{
				Tuple tp;
				if(factlist[j].getHead() == schemelist[i].getHead())
				{
					vector<string> temp;
					temp = factlist[j].getParalist();

					for(int k = 0; k < temp.size(); k++)
					{
						tp.push_back(temp[k]);
					}
					ra.add_to_Tuples(tp);
				}
			}

			relationlist.push_back(ra);

		}
	}

	void processRule1()
	{
		int before = 0;
		int after = 1;

		while(countTuple(before, after))		// to check if the number of Tuples changed
		{

			before = sizeTuple();
			for(int i = 0; i < rulelist.size(); i++)		// union with the same relation in Database
			{
				Relation temp;
				Relation JR;

				processRule2(i, JR, temp);

			}

			cout << "number of tuples in database after " << counter+1 << " iteration: " << sizeTuple() << endl;
			after = sizeTuple();
			counter ++;
		}
	}

	void processRule2(int &i, Relation &JR, Relation &temp)
	{
		for(int j = 0; j < rulelist[i].getBodylist().size(); j++)
		{
			for(int k = 0; k < relationlist.size(); k++)
			{
				if(rulelist[i].getBodylist().at(j).getHead() == relationlist[k].getName())
				{
					if(j == 0)
					{
						JR = queryAttribute(i, j, k);
					}
					else
					{
						temp = queryAttribute(i, j, k);		// this need to be joined with JR
						JR = JR.join(JR, temp);			// joining JR and temp results in new JR
					}
					// up to here, select, project, rename, join are done

					
				}
			}
		}
			
		forproject3(JR, i);		// this takes long

		forrename(JR);

		forunion(JR);

	}
	
	void forunion(Relation &JR)
	{
		//cout << "forunion " << endl;

		bool exist = false;
		for(int i = 0; i < relationlist.size(); i++)
		{
			if(JR.getName() == relationlist[i].getName())
			{
				exist = true;
				relationlist[i] = JR.myunion(JR, relationlist[i]);
			}
		}
		if(!exist)
		{
			relationlist.push_back(JR);
		}
	}

	void forrename(Relation &JR)
	{

		//cout << "forrename " << endl;
		for(int i = 0; i < relationlist.size(); i++)
		{
			if(JR.getName() == relationlist[i].getName())
			{
				for(int j = 0; j < JR.getSchema().getSchema().size(); j ++)
				{
					JR = JR.rename(j, relationlist[i].getSchema().getSchema().at(j));
				}
			}
		}
	}
	void forproject3(Relation &JR, int &i)
	{
		//cout << "forproject3 " << endl;

		JR.setName(rulelist[i].getRulehead().getHead());
		projected.clear();
		vector<string> tempschema;
		vector<int> index;
		index.clear();
		for(int j = 0; j < rulelist[i].getRulehead().para_size(); j++)
		{
			for(int k = 0; k < JR.getSchema().getSchema().size(); k++)
			{
				if(rulelist[i].getRulehead().getParalist().at(j) == JR.getSchema().getSchema().at(k))
				{
					//cout << " k "  << k << endl;
					//cout << " what is in k ? " << JR.getSchema().getSchema().at(k) << endl;
					index.push_back(k);

					tempschema.push_back(JR.getSchema().getSchema().at(k));
					
//					JR = JR.project(k);

				}
			}			
		}

		JR = JR.project(index);

		//for(int z = 0; z < index.size(); z++)
		//{
		//	JR = JR.project(index.at(z));
		//	projected.push_back(JR.getProjected());
		//}

		Schema sm;
		sm.setSchema(tempschema);
		JR.setSchema(sm);
		//JR.clearTuples();
		//tuplemaker(JR);

		//cout << "JR's name: " << JR.getName() << endl;
		//cout << "JR's first scheme: " << JR.getSchema().getSchema().at(0) << endl;

	}
	//void tuplemaker(Relation &JR)
	//{
	//	cout << "tuplemaker " << endl;
	//	for(int i = 0; i < JR.getProjected().size(); i++)
	//	{
	//		Tuple tp;
	//		for(int j = 0; j < projected.size(); j++)
	//		{
	//			//cout << " what is TUPLE: " << projected[j].at(i) << endl;
	//			tp.push_back(projected[j].at(i));
	//		}

	//		JR.add_to_Tuples(tp);
	//		//cout << "JR's tuple size: " << JR.getTuples().size() << endl;
	//	
	//	}
	//	cout << "end of tuplemaker " << endl;
	//}
	
	Relation queryAttribute(int i, int j, int k)
	{	
		Relation result = relationlist[k];
		bodypara = rulelist[i].getBodylist().at(j).getParalist();

		vector<int> nonterminals;		// vector of nonterminals position
		vector<int> duplicates;			// vector of duplicates position

		for(int l = 0; l < bodypara.size(); l++)		// after this for loop, selection is done
		{
			if(bodypara[l][0] == '\'')		// select1 operation, find a Terminal attribute in the query and select tuples accordingly
			{
				Relation r1;
				r1 = result.select1(l, bodypara[l]);
				result = r1;
			}
			else			// for non-Terminal attirubute in the query and check for duplicate values as well
			{
				nonterminals.push_back(l);
				for(int m = l+1; m < bodypara.size(); m++)
				{
					if(bodypara[l] == bodypara[m])
					{
						duplicates.push_back(m);
						Relation r1;
						r1 = result.select2(l, m);
						result = r1;
					}
				}
			}
		}

		result = forproject1(nonterminals, duplicates, result);		// calls a function for project and rename

		return result;
	}

	Relation forproject1(vector<int> &nonterminals, vector<int> &duplicates, Relation &result)
	{
		//cout << "forproject1 " << endl;
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

	void forproject2(vector<int> &nonterminals, vector<int> &duplicates, Relation &result, int &i)
	{
		//cout << "forproject2 " << endl;

		result = result.project(nonterminals[i]);
		/*	if(result.getProjected().size() > 0)
		{
		cout << "after projected, in the vector " << result.getProjected().at(0) << endl;
		}*/
		projected.push_back(result.getProjected());

		result = result.rename(nonterminals[i], bodypara[nonterminals[i]]);
		//		cout << "what is renamed value " << result.getSchema().getSchema().at(0) << endl;

	}
	void clearProjected()
	{
		while(!projected.empty())
		{
			projected.pop_back();
		}
	}
	void setNonterminals(vector<int> &nonterminals)
	{
		nonterminal = nonterminals;
	}
	bool countTuple(int before, int after)
	{
		if(before == after)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	int sizeTuple()
	{
		int total = 0;
		for(int i = 0; i < relationlist.size(); i++)
		{
			total += relationlist[i].tuplesize();
		}
		return total;
	}

};