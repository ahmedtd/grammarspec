//calc.cpp

//Main file for the calculator app

//Taahir Ahmed
//CSCE 315 Texas A&M University

#include <list>
using std::list

#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::getline;

#include <string>
using std::string;

#include "grammarspec.h"
using grammarspec::elements::literal;
using grammarspec::results::result;
using grammarspec::results::token;

int main(int argc, char **argv)
{
	string user_input;

	literal ohai_literal("ohai");
	list<result*> *presults;

	//Main interaction loop: prompt,read,eval
	while(true)
	{
		cout << "calc>";
		getline(cin, user_input);

		presults = ohai_literal.match(cin);
		cout << "Matched " << presults->size() << " tokens." << endl;
		delete presults;
	}

	return 0;
}