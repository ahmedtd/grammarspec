#include "elements.h"

#include <istream>
#include <list>

namespace grammarspec
{
namespace elements
{

whitespace::whitespace()
:
	charset(" \t\n")
{
	
}

void whitespace::munch_whitespace(std::istream &parse_stream)
{
	whitespace wmatch;
	std::list<results::result*> *wmatch_result;
	//Be sure to disable munching whitespace in the call to match()
	//Otherwise, we'll see infinite recursion as match() calls
	//munch_whitespace() calls match() calls...
	wmatch_result = wmatch.match(parse_stream, false);
	//We own the memory pertaining to results
	delete wmatch_result;
}
	
}
}