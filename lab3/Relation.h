#pragma once

#include <string>
#include <set>
#include <vector>
#include "Tuple.h"
#include "Schema.h"

using namespace std;

class Relation
{
private:
	string name;
	Schema sm;
	set<Tuple> tuples;
	vector<string> oneprojected;


public:

	Relation()
	{
	}

	string getName()
	{
		return name;
	}
	Schema getSchema()
	{
		return sm;
	}
	vector<string> getProjected()
	{
		return oneprojected;
	}
	void setProjected(vector<string> temp)
	{
		oneprojected = temp;
	}
	void setName(string temp)
	{
		name = temp;
	}
	void setSchema(Schema temp)
	{
		sm = temp;
	}
	void setTuples(set<Tuple> temp)
	{
		tuples = temp;
	}
	void add_to_Tuples(Tuple temp)
	{
		tuples.insert(temp);
	}
	int tuplesize()
	{
		return tuples.size();
	}

	Relation rename(int position, string newname)
	{
		Relation result;
		result.setName(name);
		result.setTuples(tuples);	
		sm.rename(position, newname);		// change schema's name
		result.setSchema(sm);

		return result;
	}
	Relation select1(int position, string terminal)			// select tuples which are Terminal
	{	
		Relation result;
		result.setName(name);
		result.setSchema(sm);
		set<Tuple>::iterator iter;

		for(iter=tuples.begin(); iter!=tuples.end(); ++iter)
		{
			Tuple tp = *iter;
			if(tp[position] == terminal)
			{
				result.add_to_Tuples(tp);
			}
		}
		return result;
	}
	Relation select2(int position1, int position2)			// selec tuples which have duplicate values
	{
		Relation result;
		result.setName(name);
		result.setSchema(sm);
		set<Tuple>::iterator iter;

		for(iter=tuples.begin(); iter!=tuples.end(); ++iter)
		{
			Tuple tp = *iter;
			if(tp[position1] == tp[position2])
			{
				result.add_to_Tuples(tp);
			}
		}
		return result;

	}
	Relation project(int column)
	{
		Relation result;
		result.setName(name);
		result.setSchema(sm);
		result.setTuples(tuples);

		set<Tuple>::iterator iter;

		for(iter=tuples.begin(); iter!=tuples.end(); ++iter)
		{
			Tuple tp = *iter;
			oneprojected.push_back(tp[column]);
		}

		result.setProjected(oneprojected);

		return result;
	}


};