#include "elements.h"

namespace grammarspec
{
namespace elements
{

literal::literal(const std::string &in_literal_to_match)
:
	literal_to_match(in_literal_to_match)
{
	
}

std::list<results::result*>* literal::match(
	      std::istream &parse_stream,
	const bool          ignore_whitespace
) const
{
	std::list<results::result*> *return_list =
	new std::list<results::result*>;
	
	if(ignore_whitespace)
		whitespace::munch_whitespace(parse_stream);
	
	//TODO record the issue described here in the logbook, strike this comment
	//block

	//This, and the similar code in charset, is causing problems.  Basically,
	//get blocks when the end of cin is reached, because it's just waiting for
	//more input -- it hasn't reached EOF.  So, how do I check whether the
	//get() operation will return a character immediately?

	//Ok, the rdbuf()->in_avail() code below lets me check whether there are
	//characters to be read.  However, when I'm reading from cin, it works
	//once, then freaks out and immediately fails all subsequent matches --
	//causes the console to freak out.
	// std::stringstream trystream;
	// for(size_t count = 0; count < literal_to_match.length(); count++)
	// 	if(parse_stream.rdbuf()->in_avail())
	// 		trystream.put(parse_stream.get());
	// 	else
	// 		break;

	//This code also freaks out the console.  I need to think about how I want
	//to handle this case.  I really want to be able to parse streams as they
	//arrive, but I also need to be able to buffer user input to prevent these
	//infinite immediate no match results.

	//Probably what I'll have to do is only do checks for EOF in my code here
	//(basically parse_stream.good()), and manually buffer user input using
	//getline().  This is justifiable because line-based user input is a
	//strange beast compared to other streams, and because it induces minimal
	//overhead.  While it would be cool to do something.match(cin), I shouldn't
	//bend over backwards in this code to make it happen
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
}