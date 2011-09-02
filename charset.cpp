#include "elements.h"

#include <istream>
#include <list>
#include <set>

namespace grammarspec
{
namespace elements
{

charset::charset(const std::set<char> &in_chars_to_match, const int in_n)
:
	chars_to_match(in_chars_to_match),
	n(in_n)
{
	
}

charset::charset(const std::string &in_string, const int in_n)
:
	chars_to_match(in_string.begin(), in_string.end()),
	n(in_n)
{
	
}

std::list<results::result*>* charset::match(
	      std::istream &parse_stream,
	const bool          ignore_whitespace
) const
{
	if(ignore_whitespace)
		whitespace::munch_whitespace(parse_stream);
	
	//We need the local copy of n to allow multiple threads to execute
	//this code at once.
	std::stringstream matched_stream;
	int               n_localcopy(n);
	
	bool continue_matching = (n_localcopy != 0);
	while(continue_matching && parse_stream.good())
	{
		if(n_localcopy > 0)
		{
			n_localcopy--;
			if(n_localcopy==0)
				continue_matching = false;
		}
		
		char c = parse_stream.peek();
		if(chars_to_match.find(c) != chars_to_match.end())
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
		return_list->push_back(new results::token(matched_stream.str()));
	
	return return_list;
}

}
}
