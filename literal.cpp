/// \file literal.cpp
/// Implementation file for \ref grammarspec::literal and
/// \ref grammarspec::literal::data

#include "grammarspec.h"

#include <istream>
#include <sstream>
#include <string>

namespace grammarspec
{

literal::literal(const std::string &in_literal_to_match)
:
	literal_to_match(in_literal_to_match)
{
	
}

literal::data* literal::match(std::istream &parse_stream) const
{	
	std::stringstream trystream;
	for(size_t count = 0; count < literal_to_match.length(); count++)
		if(parse_stream.good())
			trystream.put(parse_stream.get());
		else
			break;
	
	std::string trystring(trystream.str());
	
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
