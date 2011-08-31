//grammarspec.h

//Main include file for a small grammer specification library

#ifndef GRAMMARSPEC
#define GRAMMARSPEC

#include <set>
#include <sstream>
#include <string>

// WARNING: when defining a grammar tree, it must all be in the same scope
// Otherwise, the pointer links will break

namespace grammarspec
{

//Abstract base class for parse elements: requires match functionality	
class element
{
public:
	virtual result* match(const std::istream &parse_stream) const = 0;
};

//Abstract base class for parse results
class result
{
public:
	result(const std::string *in_original)
	:
		original(in_original)
	{
		
	}

	virtual ~result()
	{
		delete original;
	}

	const virtual std::string result_type() const = 0;
	const virtual void * result() const = 0;

	//Get the original substring corresponding to this result
	const virtual std::string* original()
	{
		return original;
	}
private:

	//The original substring corresponding to this result
	const std::string *original;
};

class result_token : public result
{
public:
	result_token(const std::string *original)
	:
		result(original)
	{
		
	}

	const virtual std::string result_type() const
	{
		return std::string("string:token");
	}

	const virtual std::string* result() const
	{
		return original;
	}	
};

class result_vector : public result
{
public:
	result_vector(const std::string *original)
	:
		result(original)
	{
		
	}

	const virtual std::string result_type() const
	{
		return std::string("vector:result");
	}

	const virtual vector<result *>* result() const
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

	// TODO: update to properly return result *
	virtual int result_vector* match(const std::string &text,
	                                 int match_start_index) const
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

	// TODO: update to properly return result*
	virtual int result_vector* match(const std::string &text,
	                                 int match_start_index) const
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

//match up to n characters from chars_to_match
//if n==-1, match any number of chars
class charset : public element
{
public:
	charset(const std::set<char> &in_chars_to_match, int in_n = -1)
	:
		chars_to_match(in_chars_to_match),
		n(in_n)
	{
		
	}

	virtual result_token* match(const std::istream &parse_stream) const
	{
		std::sstream matched_stream;

		bool continue_matching = (chars_to_match != 0);
		while(continue_matching)
		{
			if(n > 0)
			{
				n--;
				if(n==0)
					continue_matching = false;
			}

			char c = parse_stream.peek();
			if(chars_to_match.find(c) != set<char>::end)
			{
				parse_stream.get();
				matched_stream.put(c);
			}
			else
				break;
		}

		if(matched_stream.str().length() != 0)
			return new result_token(new string(matched_stream.str()));
		else
			return NULL;
	}
private:
	std::set<char> chars_to_match;
	int n;
};

class whitespace : public charset
{
public:
	whitespace()
	:
		charset(" \t\n")
	{
		
	}	
};

class literal : public element
{
public:
	stringliteral(const std::string &in_literal_to_match,
	              bool in_whitespace_allowed = true)
	:
		literal_to_match(in_literal_to_match),
		whitespace_allowed(in_whitespace_allowed)
	{
		
	}

	//Returns the new match_start_index.  If this is not changed, then there
	//was no match
	virtual result_token* match(const std::istream &parse_stream) const
	{
		if(whitespace_allowed)
		{
			whitespace wmatch;
			result *wmatch_result = wmatch.match(parse_stream);

			if(result != NULL)
				delete result; //We own the memory pertaining to results
		}

		// I need to nondestructively get the next literal_to_match.length()
		// characters from the stream;

		// get() them, then use putback()

		if(index_of_literal == match_start_index)
			return new result_token(text);
		else
			return NULL;
	}

private:
	std::string literal_to_match;
	bool   whitespace_allowed;
};

}//end namespace grammarspec

#endif