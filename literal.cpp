#include "elements.h"

namespace grammarspec
{
namespace elements
{

stringliteral::stringliteral(const std::string &in_literal_to_match,
                             bool in_whitespace_allowed = true)
:
	literal_to_match(in_literal_to_match)
{
	
}

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
	
}
}