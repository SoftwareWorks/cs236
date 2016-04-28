#pragma once

#include <string>
#include <vector>

class Tuple:public vector<string>
{

private:

public:

	Tuple project(vector<int> &index)
	{
		Tuple temp;

		for(int i = 0; i < index.size(); i++)
		{
			temp.push_back(at(index.at(i)));
		}
		return temp;
	}

};