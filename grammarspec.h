//grammarspec.h

//Main include file for a small grammer specification library

#ifndef GRAMMARSPEC
#define GRAMMARSPEC

#include <string>
using std::string;

// WARNING: when defining a grammar tree, it must all be in the same scope
// Otherwise, the pointer links will break

namespace grammarspec
{

//Abstract base class for parse elements: requires match functionality	
class element
{
public:
	const virtual result* match(const string &text,
	                            int match_start_index) const = 0;
};

//Abstract base class for parse results
class result
{
public:
	result(const string *in_original)
	:
		original(in_original)
	{
		
	}

	virtual ~result()
	{
		delete original;
	}

	const virtual string result_type() const = 0;
	const virtual void * result() const = 0;

	//Get the original substring corresponding to this result
	const virtual string* original()
	{
		return original;
	}
private:

	//The original substring corresponding to this result
	const string *original;
};

class result_token : public result
{
public:
	result_token(const string *original)
	:
		result(original)
	{
		
	}

	const virtual string result_type() const
	{
		return string("string:token");
	}

	const virtual string* result() const
	{
		return original;
	}	
};

class result_vector : public result
{
public:
	result_vector(const string *original)
	:
		result(original)
	{
		
	}

	const virtual string result_type() const
	{
		return string("vector:result");
	}

	const virtual vector<result *> *result() const
	{
		
	}
};

//Element to match arg1 AND arg2
class op_and : element
{
public:
	op_and(const element *in_arg1, const element *in_arg2)
	:
		arg1(in_arg1),
		arg2(in_arg2)
	{
		
	}

	int match(const string &text, int match_start_index) const
	{
		int arg1_match_index = arg1->match(text, match_start_index);
		if(arg1_match_index == match_start_index)
			return match_start_index;
		
		int arg2_match_index = arg2->match(text, arg1_match_index);
		if(arg2_match_index == arg1_match_index)
			return match_start_index;

		return arg2_match_index;
	}

private:
	const element *arg1;
	const element *arg2;
};

class op_morethan : public element
{
public:
	op_morethan(const element *in_element_to_match, int n=0)
	:
		element_to_match(in_element_to_match)
	{
		
	}

	int match(const string &text, int match_start_index) const
	{
		int match_new_index = 0;
		do
		{
			match_new_index = element_to_match->match(text, match_start_index);
			
			if(match_new_index != match_start_index)
				match_start_index = match_new_index;
		}
		while(match_new_index != match_start_index);

		return match_new_index;
	}

private:
	const element *element_to_match;
};

class stringliteral : public element
{
public:
	stringliteral(const string &in_literal_to_match)
	:
		literal_to_match(in_literal_to_match)
	{
		
	}

	//Returns the new match_start_index.  If this is not changed, then there
	//was no match
	virtual result_token* match(const string &text,
	                            int match_start_index) const
	{
		int index_of_literal = text.find(literal_to_match, match_start_index);

		if(index_of_literal == match_start_index)
			return new result_token(original);
		else
			return NULL;
	}

private:
	string literal_to_match;
};

}//end namespace grammarspec

#endif