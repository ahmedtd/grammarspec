//calc.cpp

//Main file for the calculator app

//Taahir Ahmed
//CSCE 315 Texas A&M University

#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::getline;

#include <string>
using std::string;

#include "grammarspec.h"

int main(int argc, char **argv)
{
	string user_input;

	grammarspec::elements::literal ohai_literal("ohai");
	vector<grammarspec::results::result*> *presults;

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