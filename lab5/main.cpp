#include "scanner.h"
#include "token.h"
#include "Parser.h"
#include "Datalog.h"
#include "Query.h"
#include "Rule.h"
#include "Scheme.h"
#include "Fact.h"
#include "Database.h"
#include "Schema.h"
#include "DBmanager.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{

	ofstream out_file(argv[2]);
	
	if(argc != 3)
	{
		cout << "wrong number of arguments" << endl;
	}
	else
	{	
//		scan.createFile(argv[2]);
		try
		{

			scanner scan;
			scan.getFile(argv[1]);
			scan.getToken();
			
			Parser pars(scan.accessToken());
			pars.check_Datalog();
			Scheme sc(pars.accessSchemelist());
			Fact fa(pars.accessFactlist());
			Query qu(pars.accessQuerylist());

			Datalog dlog(sc, fa, qu, pars.accessDomain(), pars.accessRulelist());


//			cout << dlog.toString();
	//		out_file << dlog.toString();

			DBmanager dm(sc, fa, pars.accessRulelist(), qu);

			dm.makeRelation();
			dm.makeQueryOrder();
			dm.creatingToDatabase();

			Database db(qu, dm);
			db.reconstruct();
			cout << db.toMain();
			out_file << db.toMain();


		}
		catch(string error_result)
		{
			out_file << "Failure!" << endl;
			out_file << error_result << endl;
		}



	}


	return 0;
}
