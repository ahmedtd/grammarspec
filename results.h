
#ifndef RESULTS
#define RESULTS

#include <list>
#include <string>

//This system should be bidirectional: I should be able to put data into
//results and "unparse" them onto a stream

namespace grammarspec
{
namespace results
{

//Abstract base class for parse results
class result
{
public:
	result(const std::string &in_original)
	:
		original(in_original)
	{
		
	}

	virtual ~result()
	{
	
	}

	//Return a pointer to the parsed result.  The user must use dynamic_cast
	//and typeid to figure out what child of results::result that they are
	//handling, and then expect the correct pointer type.  The result's memory
	//still belongs to the result object, and will be deleted when the object
	//dies
	//const virtual void * matched_item() const = 0;
		//The inheritance mechanism here doesn't really provide what I want
		//Perhaps this method should not be defined in a base calss at all, but
		//rather declared with the proper return value in each subclass


	//Get the original substring corresponding to this result
	const virtual std::string& get_original()
	{
		return original;
	}
protected:
	//The original substring corresponding to this result
	const std::string original;
};

class token : public result
{
public:
	token(const std::string &in_original)
	:
		result(in_original)
	{
		
	}
	
	//Return the string that this token has matched.
	const virtual std::string* matched_item() const
	{
		return &original;
	}
};

// class list : public result
// {
// public:
// 	list(const std::string &in_original)
// 	:
// 		result(in_original)
// 	{
		
// 	}

// 	const virtual list<result *>* result() const
// 	{
		
// 	}

// private:
// };

}
}

#endif