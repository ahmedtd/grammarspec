//calc.cpp

//Main file for the calculator app

//Taahir Ahmed
//CSCE 315 Texas A&M University

#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::getline;

#include <list>
using std::list;

#include <sstream>
using std::stringstream;

#include <string>
using std::string;

#include "grammarspec.h"
using grammarspec::elements::charset;
using grammarspec::elements::literal;
using grammarspec::results::result;
using grammarspec::results::token;

//TODO make a prefixexpression class, inheriting from element, that performs
//the stack operations in its match function

int main(int argc, char **argv)
{
	string user_input;

	charset cool_charset("ab");
	literal ohai_literal("ohai");

	list<result*> *presults;

	//Main interaction loop: prompt,read,eval
	while(true)
	{
		cout << "calc>";
		getline(cin, user_input);
		stringstream line_stream(user_input);

		presults = cool_charset.match(line_stream);

		cout << "Matched " << presults->size() << " tokens." << endl;
		delete presults;
	}

	return 0;
}