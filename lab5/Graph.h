#pragma once

#include <string>
#include <map>
#include "Node.h"
#include <stack>
#include <sstream>

using namespace std;

class Graph
{
private:
	map<string, Node> mymap;
	int postorder;
	vector<string> ruleorder;
	map<string, Node> backward;
	vector<int> ruleorderint;

public:
	Graph()
	{
		mymap.clear();
	}

	void add_to_map(string ID, Node dependents)
	{
		mymap.insert(pair<string, Node> (ID, dependents));
	}
	vector<string> getRuleorder()
	{
		return ruleorder;
	}
	map<string, Node> getmymap()
	{
		return mymap;
	}
	map<string, Node> getBackward()
	{
		return backward;
	}
	vector<int> getRuleorderint()
	{
		return ruleorderint;
	}

	void resetVisited()
	{
		ruleorder.clear();
		backward.clear();
		map<string, Node>::iterator it;

		for(it = mymap.begin(); it != mymap.end(); ++it)
		{
			it->second.resetVisited();
			it->second.resetPostorder();
		}

	}

	void DFS(Node &temp, int temp_order)
	{
		postorder = temp_order;
		temp.markVisited();
		mymap.at(temp.getID()).markVisited();
		set<string> dependents;
		dependents = temp.getDependents();
		set<string>::iterator iter;
		Node backwardedge;

		for(iter=dependents.begin(); iter!=dependents.end(); ++iter)
		{
			string ID = *iter;
			if(!mymap.at(ID).getVisited())
			{
				ruleorder.push_back(ID);
				Node current = mymap.at(ID);
				DFS(current, postorder);
			}
			else if(mymap.at(ID).getVisited() && mymap.at(ID).getPostorder() == 0)
			{
				backwardedge.add_to_set(ID);
			}
		}
		if(backwardedge.getDependents().size() > 0)
		{
			backward.insert(pair<string, Node> (temp.getID(), backwardedge));
		}
		temp.setPostorder(postorder);
		mymap.at(temp.getID()).setPostorder(postorder);
//		cout << temp.getID() << "  " << temp.getPostorder() << endl;
		postorder ++;
	}
	void topologicalSort()
	{
		ruleorderint.clear();
		int first = 1;
		for(int i = 0; i < ruleorder.size(); i++)
		{
			map<string, Node>::iterator it;
			for(it = mymap.begin(); it != mymap.end(); ++it)
			{
				if(it->second.getPostorder() == first)
				{
					ruleorder[first-1] = it->second.getID();
					string temp = "";
					temp = it->second.getID();
					temp = temp.substr(1,temp.length()-1);
					
					//cout << "temp " << temp << endl;
					//cout << "int temp " << string_to_int(temp) << endl;

					ruleorderint.push_back(string_to_int(temp)-1);

				}
			}
			first++;
		}
		//cout << "ruleorderint size " << ruleorderint.size() << endl;

	}
	string printBackward()
	{
		string output = "";

		map<string, Node>::iterator it;

		for(it = backward.begin(); it != backward.end(); ++it)
		{
			output = output + "    " + it->first + ":" + it->second.toStringset() + "\n";
		}

		return output;
	}

	int string_to_int(string s)
	{
		istringstream strm;
		strm.str(s);
		int n = 0;
		strm >> n;
		return n;
	}
//dfs(Node x)
//{
//	mark x visited
//		for each node y is adjacent to x
//		{
//			if node y is not marked
//				dfs(y)
//			end if
//		}
//}
//
//dfsForest()
//{
//	for each Node x
//	{
//		if node x is not marked
//			dfs(x)
//		end if
//	}
//}
//
//isCyclic()
//{
//	run dfsForest()
//		for each edge x->y(for each node adjacent node)
//		{
//			if(x.postorder <= y.postorder)
//				return true
//		}
//	return false
//}

};


