#include "elements.h"

namespace grammarspec
{
namespace elements
{

whitespace::whitespace()
:
	charset(" \t\n")
{
	
}

static void whitespace::munch_whitespace(const std::istream &parse_stream)
{
	whitespace wmatch;
	result_token *wmatch_result = wmatch.match(parse_stream, false);
	//Be sure to disable munching whitespace in the call to match()
	//Otherwise, we'll see infinite recursion as match() calls
	//munch_whitespace() calls match() calls...
	
	if(result != NULL)
		delete wmatch_result; //We own the memory pertaining to results
}
	
}
}