#pragma once

#include <string>
#include <vector>
#include "Scheme.h"
#include "Fact.h"
#include "Query.h"
#include "Relation.h"
#include <iostream>
#include "Tuple.h"

class DBmanager			// this class creates Database
{
private:
	vector<Predicate> schemelist;
	vector<Predicate> factlist;
	vector<Predicate> querylist;
	Scheme sc;
	Fact fa;
	vector<Relation> relationlist;
	
	

public:


	DBmanager()
	{
	}
	DBmanager(Scheme temp1, Fact temp2)
	{
		sc = temp1;
		fa = temp2;
		schemelist = sc.getSchemelist();
		factlist = fa.getFactlist();

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

			cout << schemelist[i].getHead() << endl;



			sm.setSchema(schemelist[i].getParalist());		// set a schema for a relation from the schemelist
			
			ra.setSchema(sm);

			cout << sm.getSchema().at(0) << endl;

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

};