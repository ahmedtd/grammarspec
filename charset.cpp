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

	//Conditionally match whitespace at head of stream
	//Save the result in case we need to unmatch it
	std::list<results::result*> *wmatch_result_list;
	if(ignore_whitespace)
	{
		whitespace wmatch;
		wmatch_result_list = wmatch.match(parse_stream, false);
	}
	
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
	{
		//Match success
		return_list->push_back(new results::token(matched_stream.str()));
	}
	else
	{
		//Match failure
		//Rewind any whitespace we've taken
		std::list<results::result*>::reverse_iterator
			wmatch_rit(  wmatch_result->rbegin());
		std::list<results::result*>::reverse_iterator
			wmatch_rend( wmatch_result->rend()  );
		for(; wmatch_rit != wmatch_rend; wmatch_rit++)
		{
			results::result *white_ = (wmatch_rit);
			std::string::reverse_iterator string_rit((*wmatch_rit)->rbegin());

			for(string_rit = ())
		}
	}
		
	delete wmatch_result;
	return return_list;
}

}
}
