#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>
#include <iostream>
#include <ctype.h>

#include "token.h"

using namespace std;

class scanner
{
private:
	
	int line;
	bool error;
	int error_line;	
	ifstream in_file;
	ofstream out_file;

	vector<token> vec;

public:

	scanner()
	{
		line = 1;
		error = false;
	}

	void getFile(string arg1)
	{
		in_file.open(arg1.c_str());
	}

	void createFile(string arg2)
	{
		out_file.open(arg2.c_str());

		if(error)
		{
			out_file << "Error on line " << error_line << endl;
		}
		else
		{
			for(int i = 0; i < vec.size(); i++)
			{
				out_file << "(" << vec[i].getType() << "," << vec[i].getValue() << "," << vec[i].getline() << ")" << endl;
			}
			out_file << "Total Tokens = " << vec.size() << endl;
		}

	}

	void getToken()
	{	
		string line_contents;
		string tempword = "";
		int quotation_count = 0;

		while(getline(in_file, line_contents) && !error)
		{
			cout << "line_contents: " << line_contents << endl;

			for(int i = 0; i < line_contents.size(); i++)
			{
				string character;
				character = line_contents[i];

				checkString(line_contents, tempword, character, i, quotation_count);

			}
	
			line ++;
		}
	}


	void checkString(string& line_contents, string& tempword, string& character, int& i, int& quotation_count)
	{
		if(quotation_count == 1)
		{
			if(character == "'")		// make string token
			{
				quotation_count = 0;
				token make_token("STRING","\"" + tempword + "\"",line);
				vec.push_back(make_token);
				tempword = "";
			}
		
			else if(i+1 < line_contents.size())			// add onto tempword until find another '
			{
				tempword = tempword + character;
			}
			else			// error
			{
				error_line = line;
				error = true;
			}
				
		}
		else
		{
			checkColon(line_contents, tempword, character, i, quotation_count);
		
		}
	}

	void checkColon(string& line_contents, string& tempword, string& character, int& i, int& quotation_count)
	{
		if(character == ":")
		{
			if(i+1 < line_contents.size() && line_contents[i+1] == '-')		// make token for :-
			{
				makeKeyword(line_contents, tempword, character, i, quotation_count);
				token make_token("COLON_DASH","\":-\"",line);
				vec.push_back(make_token);
				i++;
				
			}
			else			// make token for :
			{
				makeKeyword(line_contents, tempword, character, i, quotation_count);
				token make_token("COLON","\":\"",line);
				vec.push_back(make_token);
				
			}
		}
		else
		{
			checkPunc1(line_contents, tempword, character, i, quotation_count);
		}
	}

	void checkPunc1(string& line_contents, string& tempword, string& character, int& i, int& quotation_count)
	{
		if(character == "#")
		{
			i = line_contents.size();
		}
		else if(character == ",")
		{
			makeKeyword(line_contents, tempword, character, i, quotation_count);
			token make_token("COMMA","\",\"",line);
			vec.push_back(make_token);
			
		}
		else if(character ==".")
		{
			makeKeyword(line_contents, tempword, character, i, quotation_count);
			token make_token("PERIOD","\".\"",line);
			vec.push_back(make_token);
		
		}
		else if(character == "?")
		{
			makeKeyword(line_contents, tempword, character, i, quotation_count);
			token make_token("Q_MARK","\"?\"",line);
			vec.push_back(make_token);
			
		}
		else
		{
			checkPunc2(line_contents, tempword, character, i, quotation_count);
		}
	}

	void checkPunc2(string& line_contents, string& tempword, string& character, int& i, int& quotation_count)
	{
		if(character == "(")
		{
			makeKeyword(line_contents, tempword, character, i, quotation_count);
			token make_token("LEFT_PAREN","\"(\"",line);
			vec.push_back(make_token);

		}
		else if(character == ")")
		{
			makeKeyword(line_contents, tempword, character, i, quotation_count);
			token make_token("RIGHT_PAREN","\")\"",line);
			vec.push_back(make_token);

		}
		else if(character == "\'")
		{
			if(i+1 >= line_contents.size())
			{
				error_line = line;
				error = true;
			}
			else
			{
				makeKeyword(line_contents, tempword, character, i, quotation_count);	// check if tempword == Schemes, Facts, Rules, Queries, or id
				quotation_count++;
			}
			
		}
		else
		{
			checkID(line_contents, tempword, character, i, quotation_count);
		}
		
	}

	void checkID(string& line_contents, string& tempword, string& character, int& i, int& quotation_count)
	{
		if(isdigit(character[0]))	// check that string tempword is a valid ID		
		{
			if(isalpha(tempword[0]))	// check the tempword at position 0, and make sure that there is a letter already
			{
				tempword = tempword + character;
				if(i+1 >= line_contents.size())
				{
					makeKeyword(line_contents, tempword, character, i, quotation_count);
				}
			}
			else
			{
				error_line = line;
				error = true;
			}
		}
		else if(isalpha(character[0]))
		{
			tempword = tempword + character;
			if(i+1 >= line_contents.size())
			{
				makeKeyword(line_contents, tempword, character, i, quotation_count);
			}
			
		}
		else if(isspace(character[0]))   // make an ID or a keyword
		{
			if(!tempword.empty())
			{
				makeKeyword(line_contents, tempword, character, i, quotation_count);  // check if tempword == Schemes, Facts, Rules, Queries, or id
			}
		}
	
		else  		// error
		{
			error_line = line;
			error = true;
		}
	}

	void makeKeyword(string& line_contents, string& tempword, string& character, int& i, int& quotation_count)
	{
		if(tempword == "Schemes")
		{
			token make_token("SCHEMES","\"Schemes\"",line);
			vec.push_back(make_token);
			tempword = "";
		}
		else if(tempword == "Facts")
		{
			token make_token("FACTS","\"Facts\"",line);
			vec.push_back(make_token);
			tempword = "";
		}
		else if(tempword == "Rules")
		{
			token make_token("RULES","\"Rules\"",line);
			vec.push_back(make_token);
			tempword = "";
		}
		else if(tempword == "Queries")
		{
			token make_token("QUERIES","\"Queries\"",line);
			vec.push_back(make_token);
			tempword = "";
		}
		else		// make token for ID
		{
			if(!tempword.empty())
			{
				token make_token("ID","\"" + tempword + "\"",line);
				vec.push_back(make_token);
				tempword = "";
			}
		}
	}



	//void getKeyword()
	//{

	//	if(word == "Schemes")
	//	{
	//		token make_token("SCHEMES","Schemes",line);
	//		vec.push_back(make_token);
	//	}
	//	else if(word == "Facts")
	//	{
	//		token make_token("FACTS","Facts",line);
	//		vec.push_back(make_token);
	//	}
	//	else if(word == "Rules")
	//	{
	//		token make_token("RULES","Rules",line);
	//		vec.push_back(make_token);
	//	}
	//	else if(word == "Queries")
	//	{
	//		token make_token("QUERIES","Querie",line);
	//		vec.push_back(make_token);
	//	}
	//	else
	//	{
	//		
	//	}
	//}

	//void getPunctuation(char letter)
	//{
	//	if(letter == ',')
	//	{
	//		token make_token("COMMA","','",line);
	//		vec.push_back(make_token);
	//	}
	//	else if(letter == '.')
	//	{
	//		token make_token("PERIOD","'.'",line);
	//		vec.push_back(make_token);
	//	}
	//	else if(letter == '(')
	//	{
	//		token make_token("LEFT_PAREN","'('",line);
	//		vec.push_back(make_token);
	//	}
	//	else if(letter == ')')
	//	{
	//		token make_token("RIGHT_PAREN","')'",line);
	//		vec.push_back(make_token);
	//	}
	//	else if(letter == '?')
	//	{
	//		token make_token("Q_MARK","'?'",line);
	//		vec.push_back(make_token);
	//	}
	//	else
	//	{

	//	}
	//}

	//void getColon(char letter)
	//{
	//	if(letter == ':')
	//	{
	//		if(char_list.front() == "-")
	//		{
	//			char_list.pop();
	//			token make_token("COLON_DASH","':-'",line);
	//			vec.push_back(make_token);
	//		}
	//		else
	//		{
	//			token make_token("COLON","':'",line);
	//			vec.push_back(make_token);
	//		}
	//	}

	//	else
	//	{

	//	}
	//}

};