#pragma once

#include <string>
#include <set>

using namespace std;

class Node
{
private:
	set<string> dependentIDs;
	bool visited;
	int postorder;
	string ID;

public:
	Node()
	{
		dependentIDs.clear();
		visited = false;
		postorder = 0;
	}
	set<string> getDependents()
	{
		return dependentIDs;
	}
	string getID()
	{
		return ID;
	}
	void setID(string temp)
	{
		ID = temp;
	}
	void add_to_set(string dependents)
	{
		dependentIDs.insert(dependents);
	}
	bool getVisited()
	{
		return visited;
	}
	void resetVisited()
	{
		visited = false;
	}
	void resetPostorder()
	{
		postorder = 0;
	}
	void markVisited()
	{
		visited = true;
	}
	int getPostorder()
	{
		return postorder;
	}
	string postorderString(int number)
	{
		stringstream ss;
		ss << number;
		return ss.str();
	}

	void setPostorder(int temp)
	{
		postorder = temp;
	}
	string toStringset()
	{
		string output = "";
		set<string>::iterator iter;
		for(iter=dependentIDs.begin(); iter!=dependentIDs.end(); ++iter)
		{
			string temp = *iter;
			output = output + " " + temp;
		}
		return output;
	}

};