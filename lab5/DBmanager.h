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
#include "Graph.h"
#include <sstream>

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
	Query qu;
	Graph mygraph;
	vector<string> postorders;
	vector<string> ruleorders;
	vector<string> backwardedges;
	vector<string> evaluations;

public:

	DBmanager()
	{
	}
	DBmanager(Scheme temp1, Fact &temp2, vector<Rule> temp3, Query temp4)
	{
		counter = 0;
		sc = temp1;
		fa = temp2;
		schemelist = sc.getSchemelist();
		factlist = fa.getFactlist();
		rulelist = temp3;
		qu = temp4;
		querylist = qu.getQuerylist();

	}

	vector<string> getPostorders()
	{
		return postorders;
	}
	vector<string> getRuleorders()
	{
		return ruleorders;
	}
	vector<string> getBackwardedges()
	{
		return backwardedges;
	}
	vector<string> getEvaluations()
	{
		return evaluations;
	}

	int getCounter()
	{
		return counter;
	}
	vector<Relation> getRelationlist()
	{
		return relationlist;
	}
	void setGraph(Graph &temp)
	{
		mygraph = temp;
	}
	Graph getGraph()
	{
		return mygraph;
	}

	string printGraph()
	{
		string graph = "";

		graph = "Dependency Graph\n";
		map<string, Node> mymap;
		mymap = mygraph.getmymap();

		map<string, Node>::iterator it;

		for(it = mymap.begin(); it != mymap.end(); ++it)
		{
			pair<string, Node> current = *it;

			graph = graph + "  " + current.first + ":" +  current.second.toStringset() + "\n";
		}
		graph = graph + "\n";

		return graph;
	}

	void makeQueryOrder()
	{
		Graph newgraph;
		stringstream convertingquery;
		stringstream convertingrule;

		for(int i = 0; i < querylist.size(); i++)
		{
			convertingquery << "Q" << i+1;
			Node temp;
			temp.setID(convertingquery.str());

			for(int j = 0; j < rulelist.size(); j++)
			{
				convertingrule << "R" << j+1;
				if(querylist.at(i).getHead() == rulelist.at(j).getRulehead().getHead())
				{
					temp.add_to_set(convertingrule.str());
				}
				convertingrule.str("");
			}
			newgraph.add_to_map(convertingquery.str(), temp);		
			
			convertingquery.str("");
		}

		makeRuleOrder(newgraph);		// after this line, Dependency graph is created.
		setGraph(newgraph);
	}
	void makeRuleOrder(Graph &newgraph)
	{
		stringstream convertingkey;
		stringstream convertingrule;

		for(int i = 0; i < rulelist.size(); i++)
		{
			convertingkey << "R" << i+1;
			Node temp;
			temp.setID(convertingkey.str());

			for(int j = 0; j < rulelist.at(i).getBodylist().size(); j++)
			{
				for(int k = 0; k < rulelist.size(); k++)
				{
					convertingrule << "R" << k+1;

					if(rulelist.at(i).getBodylist().at(j).getHead() == rulelist.at(k).getRulehead().getHead())
					{
						temp.add_to_set(convertingrule.str());
					}
					convertingrule.str("");
				}
			}
			newgraph.add_to_map(convertingkey.str(), temp);		

			convertingkey.str("");
		}

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


	void printPostorder(int i)
	{
		string output = "";
		mygraph.resetVisited();
		map<string, Node> mymap;
		mymap = mygraph.getmymap();
		
		stringstream convertingquery;
		convertingquery << "Q" << i+1;

		Node current = mymap.at(convertingquery.str());
		
		mygraph.DFS(current, 1);

		mymap = mygraph.getmymap();

		map<string, Node>::iterator it;

		for(it = mymap.begin(); it != mymap.end(); ++it)
		{
			if(it->second.getVisited())
			{
				output = output + "    " + it->first + ": " + it->second.postorderString(it->second.getPostorder()) + "\n";
			}
		}

		convertingquery.str("");
		postorders.push_back(output);
	}

	void printRuleorder()
	{
		string output = "";
		mygraph.topologicalSort();
		vector<string> temp = mygraph.getRuleorder();

		for(int i = 0; i < temp.size(); i++)
		{
			output = output + "    " + temp[i] + "\n";
		}
		ruleorders.push_back(output);
	}

	void creatingToDatabase()
	{
		for(int i = 0; i < querylist.size(); i++)
		{
			printPostorder(i);
			printRuleorder();
			backwardedges.push_back(mygraph.printBackward());
			if(mygraph.getBackward().size() == 0)		// not cyclic. ProcessRule only once
			{
				//cout << "Not Cyclic" << endl;

				smartProcessRule();
			}
			else		// cyclic. ProcessRule until fixed-point
			{
				//cout << "Cyclic" << endl;
				processRule1();
			}

		}
	}
	void smartProcessRule()
	{
		vector<int> ruleorderint = mygraph.getRuleorderint();
		string output = "";
		for(int i = 0; i < ruleorderint.size(); i++)
		{
			Relation temp;
			Relation JR;
			output = output + "  " + rulelist.at(ruleorderint.at(i)).toString();
			processRule2(ruleorderint.at(i), JR, temp);
		}
		evaluations.push_back(output);

	}
	void processRule1()
	{
		int before = 0;
		int after = 1;

		vector<int> ruleorderint = mygraph.getRuleorderint();
		string output = "";

		while(countTuple(before, after))		// to check if the number of Tuples changed
		{

			before = sizeTuple();
			for(int i = 0; i < ruleorderint.size(); i++)		// union with the same relation in Database
			{
				Relation temp;
				Relation JR;

				output = output + "  " + rulelist.at(ruleorderint.at(i)).toString();
				processRule2(ruleorderint.at(i), JR, temp);

			}
			after = sizeTuple();
			counter ++;
		}
		evaluations.push_back(output);

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
