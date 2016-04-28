#include "scanner.h"
#include "token.h"

#include <string>
#include <iostream>


using namespace std;

int main(int argc, char* argv[])
{
	scanner scan;

	if(argc != 3)
	{
		cout << "wrong number of arguments" << endl;
	}
	else
	{
		scan.getFile(argv[1]);
		scan.getToken();
		

		scan.createFile(argv[2]);
	}

	return 0;
}