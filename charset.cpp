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

std::list<results::result*>* charset::match(
	const std::istream &parse_stream,
	const bool          ignore_whitespace
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
		if(chars_to_match.find(c) != std::set<char>::end)
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

}
}
