
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

//match up to n characters from chars_to_match
//if n==-1, match any number of chars
class charset : public element
{
public:
	charset(const std::set<char> &in_chars_to_match, int in_n = -1);

	const virtual std::list<results::result*>* match(
		const std::istream &parse_stream,
	    const bool          ignore_whitespace = true
	    ) const;
private:
	const std::set<char> chars_to_match;
	const int n;
};

class whitespace : public charset
{
public:
	whitespace();

	//Convenience function to strip whitepsace from the head of the stream
	static void munch_whitespace(const std::istream &parse_stream);
};

class literal : public element
{
public:
	stringliteral(const std::string &in_literal_to_match,
	              bool in_whitespace_allowed = true);

	//If the token is matched, returns a std::list of length 1, containing a
	//pointer to a results::token object containing the token.  If a token is
	//not matched, returns an empty std::list.
	const virtual std::list<results::result*>* match(
		const std::istream &parse_stream,
	    const bool ignore_whitespace = true
	    ) const;

private:
	std::string literal_to_match;
};

}
}

#endif