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

	grammarspec::stringliteral ohai_literal("ohai");
	grammarspec::stringliteral world_literal("world");
	grammarspec::op_morethan   morethan(&ohai_literal, 1);

	//Main interaction loop: prompt,read,eval
	while(true)
	{
		cout << "calc>";
		getline(cin, user_input);

		int match_start_index = 0;
		int match_finish_index = 0;
		match_finish_index = morethan.match(user_input, match_start_index);
		
		if(match_finish_index != match_start_index)
			cout << "String matched!" << endl;
	}

	return 0;
}