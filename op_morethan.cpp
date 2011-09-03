
#ifndef OP_MORETHAN
#define OP_MORETHAN

#include <list>

#include "elements.h"
#include "results.h"

namespace grammarspec
{
namespace elements
{
	
morethan::morethan(const element &in_element_to_match, const int in_n)
:
	element_to_match(in_element_to_match),
	n(in_n)
{
	
}

std::list<results::result*>* morethan::match(
		      std::istream &parse_stream,
	    const bool          ignore_whitespace
		)
{
	
}

}
}

#endif