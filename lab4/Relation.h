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
	vector<int> joinColumn;


public:

	Relation()
	{
		oneprojected.clear();
	}

	string getName()
	{
		return name;
	}
	Schema getSchema()
	{
		return sm;
	}
	set<Tuple> getTuples()
	{
		return tuples;
	}
	vector<string> getProjected()
	{
		return oneprojected;
	}
	void setProjected(vector<string> &temp)
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
	void setTuples(set<Tuple> &temp)
	{
		tuples = temp;
	}
	void add_to_Tuples(Tuple &temp)
	{
		tuples.insert(temp);
	}
	int tuplesize()
	{
		return tuples.size();
	}
	void clearTuples()
	{
		tuples.clear();
	}

	Relation myunion(Relation &JR, Relation &DR)
	{
		DR.setName(name);
		DR.setSchema(sm);
		JR.setTuples(tuples);

		set<Tuple>::iterator iter;

		for(iter=tuples.begin(); iter!=tuples.end(); ++iter)
		{
			Tuple tp = *iter;
			DR.add_to_Tuples(tp);
		}
		
		
		return DR;
	}


	//Tuple convertTuple(vector<string> temp)
	//{
	//	Tuple newtuple;
	//	for(int i = 0; i < temp.size(); i ++)
	//	{
	//		newtuple.push_back(temp[i]);
	//	}
	//	return newtuple;
	//}
	//vector<string> combineTuple(vector<string> tp, vector<string> &addedtuples)
	//{
	//	for(int i = 0; i < addedtuples.size(); i++)
	//	{
	//		tp.push_back(addedtuples[i]);
	//	}
	//	return tp;
	//}
	//bool compareSchema(vector<string> &JRschema, vector<string> &tempschema)
	//{
	//	for(int i = 0; i < JRschema.size(); i++)
	//	{
	//		if(JRschema[i] != tempschema[i])
	//		{
	//			return false;
	//		}
	//	}
	//	return true;
	//}
	//bool compareTuple(Tuple &tp, Tuple &tp2)
	//{
	//	for(int i = 0; i < tp.size(); i++)
	//	{
	//		if(tp[i] != tp2[i])
	//		{
	//			return false;
	//		}
	//	}
	//	return true;
	//}

	//bool specialCase1(Tuple &tp, Tuple &tp2, vector<string> &JRschema, vector<string> &tempschema)
	//{
	//	if(JRschema.size() == tempschema.size())
	//	{

	//		if(compareSchema(JRschema, tempschema))
	//		{

	//			if(!compareTuple(tp, tp2))
	//			{
	//				return false;
	//			}
	//		}
	//	}
	//	return true;
	//}
	Relation join(Relation &JR, Relation &temp)
	{
		//cout << "JOIN " << endl;
		Relation emptyR;
		Schema s;
		s = combineScheme(JR.getSchema(), temp.getSchema());		// Schema combined !

		//vector<string> test;
		//test = s.getSchema();
		//cout << "combined schema:   ";
		//for(int i = 0; i < test.size(); i++)
		//{
		//	cout << test[i] << "     ";
		//}
		//cout << endl;

		emptyR.setSchema(s);

		set<Tuple> JRtuple;
		set<Tuple> temptuple;
		
		JRtuple = JR.getTuples();
		//cout << "JRtuple size: " <<JRtuple.size() << endl;

		temptuple = temp.getTuples();

		//cout << "temptuple size: " << temptuple.size() << endl;

		set<Tuple>::iterator iter;

		for(iter=JRtuple.begin(); iter!=JRtuple.end(); ++iter)
		{

//			cout << "first string in JRtuple " << tp[0] << endl;

			set<Tuple>::iterator iter2;
			for(iter2=temptuple.begin(); iter2!=temptuple.end();++iter2)
			{
				vector<string> JRschema = JR.getSchema().getSchema();
				vector<string> tempschema = temp.getSchema().getSchema();
			
				Tuple tp = *iter;
				Tuple tp2 = *iter2;
				
//				cout << "first string in temptuple " << tp2[0] << endl;
				
				vector<string> addedtuples;

				if(isjoinable(tp, tp2, JRschema, tempschema, addedtuples))
				{
					// add temp tuples to JR tuples

					//vector<string> newtuples;
					//newtuples = combineTuple(tp, addedtuples);
					//addedtuples.clear();
					//
					tp = tupleCombine(tp, tp2);

					//cout << "my new tp after join: ";
					//for(int i = 0; i < tp.size(); i++)
					//{
					//	cout << tp[i] << "  ";
					//}
					//cout << endl;
					//Tuple newtuple = convertTuple(newtuples);

					emptyR.add_to_Tuples(tp);
				}
			}

		}

		return emptyR;
	}
	Tuple tupleCombine(Tuple &tp, Tuple &tp2)
	{
		//cout << "tupleCombine " << endl;
		Tuple result = tp;
		for(int i = 0; i < joinColumn.size(); i++)
		{
			result.push_back(tp2.at(joinColumn.at(i)));
		}
		return result;
	}
	bool isjoinable(Tuple &tp, Tuple &tp2, vector<string> &JRschema, vector<string> &tempschema, vector<string> &addedtuples)
	{
		//cout << "isjoinable " << endl;
		for(int i = 0; i < JRschema.size(); i++)
		{
			for(int j = 0; j < tempschema.size(); j++)
			{
				if(JRschema[i] == tempschema[j] && tp[i] != tp2[j])
				{
					return false;
				}
			}
		}
		return true;
	}

	//bool joinable(Tuple &tp, Tuple &tp2, vector<string> &JRschema, vector<string> &tempschema, vector<string> &addedtuples)
//	{
//		if(!specialCase1(tp, tp2, JRschema, tempschema))
//		{
//			return false;
//		}
//		for(int i = 0; i < JRschema.size(); i++)
//		{
//
//			for(int j = 0; j < tempschema.size(); j++)
//			{
//
//				if(JRschema[i] == tempschema[j] && tp[i] != tp2[j])
//				{
//					return false;
//				}
//				else if(JRschema[i] == tempschema[j] && tp[i] == tp2[j])
//				{	
//
//					joinable2(tp2, addedtuples, j);
//					return true;
//				}
//			}
//		}
//
//		specialCase2(tp, tp2, JRschema, tempschema, addedtuples);		// takes care of Cartesian product
//		return true;
//	}
//	void specialCase2(Tuple &tp, Tuple &tp2, vector<string> &JRschema, vector<string> &tempschema, vector<string> &addedtuples)
//	{
//
//		//for(int i = 0; i < JRschema.size(); i++)
//		//{
//
//			for(int j = 0; j < tempschema.size(); j++)
//			{
//
//				addedtuples.push_back(tp2[j]);
//			}
////		}
//	}
//	void joinable2(Tuple &tp2, vector<string> &addedtuples, int &j)
//	{
//
//		for(int k = 0; k < tp2.size(); k++)
//		{
//			if(j != k)
//			{
//				addedtuples.push_back(tp2[k]);
//			}
//		}
//
//	}
	Schema combineScheme(Schema JRschema, Schema tempschema)		// function that combines two schemas
	{
		vector<string> jr;
		vector<string> temp;
		jr = JRschema.getSchema();
		temp = tempschema.getSchema();
		joinColumn.clear();
		for(int i = 0; i < temp.size(); i++)
		{
			bool exist = false;
			for(int j = 0; j < jr.size(); j++)
			{
				if(temp[i] == jr[j])
				{
					exist = true;
				}
			}
			if(!exist)
			{
				jr.push_back(temp[i]);
				joinColumn.push_back(i);
			}
		}
		JRschema.setSchema(jr);
		return JRschema;
	}
	Relation rename(int position, string newname)
	{
		//cout << "rename" << endl;
		Relation result;
		result.setName(name);
		result.setTuples(tuples);	
		sm.rename(position, newname);		// change schema's name
		result.setSchema(sm);

		return result;
	}
	Relation select1(int position, string terminal)			// select tuples which are Terminal
	{	
		//cout << "select1 " << endl;
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
		//cout << "select2 " << endl;
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
		//cout << "project " << endl;
		oneprojected.clear();
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
	Relation project(vector<int> &index)
	{
		Relation result;
		result.setName(name);
		result.setSchema(sm);
		result.setTuples(tuples);

		set<Tuple> newTuples;
		set<Tuple>::iterator iter;
		for(iter=tuples.begin(); iter!=tuples.end(); ++iter)
		{
			Tuple temp = *iter;
			newTuples.insert(temp.project(index));
		}

		result.setTuples(newTuples);
		
		return result;
	}


};