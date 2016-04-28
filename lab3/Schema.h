#pragma once

#include <string>
#include <vector>
#include "Predicate.h"
#include "Scheme.h"
#include "Parameter.h"

class Schema
{
private:
	vector<string> schemas;

public:
	Schema()
	{
	}
	void rename(int position, string newname)
	{
		schemas[position] = newname;
	}

	vector<string> getSchema()
	{
		return schemas;
	}

	void setSchema(vector<string> temp)
	{
		schemas = temp;
	}

};