
#ifndef ELEMENTS
#define ELEMENTS

#include <set>
#include <sstream>
#include <string>
#include <list>

#include "results.h"

namespace grammarspec
{
namespace elements
{
	
//Abstract base class for parse elements: requires match functionality	
class element
{
public:
	//Attempt to match this element at the head of parse_stream, ignoring
	//whitespace by default.  If a match was found, the return value will be
	//a std::vector of generic results::result pointers.  The caller is
	//expected to use RTTI to identify the specific type of each result in the
	//vector.  Additionally, if the caller is another child of element, it is
	//expected that they will iterate over the vector returned by this function
	//and add each element to their own results vector.  The end result of this
	//paradigm is a flat vector of generic results. (Note that some of those
	//results may be instances of results::list, which explicitly introduces
	//nesting into the hierarchy)
	const virtual std::list<results::result*>* match(
		const std::istream &parse_stream,
	    const bool ignore_whitespace = true
	) const = 0;
};

// //Element to match arg1 AND arg2
// class op_and : element
// {
// public:
// 	and(const element &in_arg1, const element &in_arg2)
// 	:
// 		arg1(in_arg1),
// 		arg2(in_arg2)
// 	{
		
// 	}

// 	virtual results::vector* match(const std::istream &parse_stream) const
// 	{
		
// 	}

// private:
// 	const element arg1;
// 	const element arg2;
// };

// class op_morethan : public element
// {
// public:
// 	morethan(const element *in_element_to_match, int n=0)
// 	:
// 		element_to_match(in_element_to_match)
// 	{
		
// 	}

// 	virtual results::vector* match(const std::istream &parse_stream) const
// 	{
		
// 	}

// private:
// 	const element *element_to_match;
// };

class whitespace;

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

	const virtual std::list<results::result*>* match(
		const std::istream &parse_stream,
	    const bool          ignore_whitespace = true
	    ) const
	{
		if(ignore_whitespace)
			whitespace::munch_whitespace(parse_stream);

		std::stringstream matched_stream;

		//To be thread-safe, we should make a local copy of n to modify

		bool continue_matching = (n != 0);
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

		std::list<results::result*> *return_list =
			new std::list<results::result*>;

		if(matched_stream.str().length() != 0)
			return_list->push_back(matched_stream.str());
		
		return return_list;
	}
private:
	const std::set<char> chars_to_match;
	const int n;
};

class whitespace : public charset
{
public:
	whitespace()
	:
		charset(" \t\n")
	{
		
	}

	//Convenience function to strip whitepsace from the head of the stream
	static void munch_whitespace(const std::istream &parse_stream)
	{
		whitespace wmatch;
		result_token *wmatch_result = wmatch.match(parse_stream, false);
			//Be sure to disable munching whitespace in the call to match()
			//Otherwise, we'll see infinite recursion as match() calls
			//munch_whitespace() calls match() calls...

		if(result != NULL)
			delete wmatch_result; //We own the memory pertaining to results
	}
};

class literal : public element
{
public:
	stringliteral(const std::string &in_literal_to_match,
	              bool in_whitespace_allowed = true)
	:
		literal_to_match(in_literal_to_match)
	{
		
	}

	//If the token is matched, returns a std::list of length 1, containing a
	//pointer to a results::token object containing the token.  If a token is
	//not matched, returns an empty std::list.
	const virtual std::list<results::result*>* match(
		const std::istream &parse_stream,
	    const bool ignore_whitespace = true
	    ) const
	{
		std::list<results::result*> *return_list =
			new std::list<results::result*>;

		if(ignore_whitespace)
			whitespace::munch_whitespace(parse_stream);

		std::stringstream trystream;
		for(int count = 0; count < literal_to_match.length(); count++)
			trystream.put(parse_stream.get());

		std::string &trystring = trystream.str();

		if(trystring.compare(literal_to_match) == 0)
			return_list->push_back(new results::token(literal_to_match));
		else
		{
			//Unwind the the characters that were read to detect the literal
			std::string::reverse_iterator rit;
			for(rit=trystring.rbegin(); rit != trystring.rend(); rit++)
				parse_stream.putback(*rit);
		}

		// To be totally correct, we might unwind the whitespace we
		// (optionally) matched upon entry to the function.  However, we
		// have either matched no whitespace or we are ignoring it.  If we
		// are ignoring it, then not replacing it is a victimless crime

		return return_list;
	}

private:
	std::string literal_to_match;
};

}
}

#endif