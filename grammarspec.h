/// \file grammarspec.h
/// Main include file for grammarspec, an object-oriented, threadsafe parsing
/// and serialization library.

#ifndef GRAMMARSPEC
#define GRAMMARSPEC

#include <istream>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <list>

//TODO implement binary function matching and integer/float matching, and
//we'll be good to go on the calculator.  Also need op_morethan, op_or, op_and

//TODO also need a generic token

/// \brief Main namespace for grammarspec
///
/// Main namespace for grammarspec, an object-oriented, threadsafe parsing
/// and serialization library.
namespace grammarspec
{

/// \brief Abstract base class for parse elements.
///
/// Abstract base class for parse elements: requires match and streamify
/// functionality of all subclasses.  To enforce threadsafety, no subclass of
/// element should have non-const members.  This property allows the same
/// element instance to be used to parse multiple streams concurrently.
class element
{
public:

	/// \brief Constructor
	///
	/// Constructor
	///
	/// \param [in] implicit_whitespace
	/// If true, then a call to match() will allow (but not require) whitespace
	/// between its sub-elements.  This removes the necessity of manually
	/// specifying whitespace in the element tree.
	element(const bool &in_implicit_whitespace = true);

	/// \brief Virtual destructor.
	///
	/// Virtual destructor.  Allows dispatching to the correct subclass
	/// destructor.
	virtual ~element();
	
	/// \brief Parse this element tree into a data tree from the given stream.
	///
	/// \param [in,out] parse_stream
	/// Attempt to match from the head of this stream.  If the match succeeds,
	/// then the matching characters will be removed from the head of the
	/// stream.  If the match fails, the stream will be unchanged.  However, it
	/// cannot be const, since characters will always be read from it, at least
	/// temporarily.
	///
	/// \return
	/// Pointer to a data element (NULL if the match failed).  If the
	/// specific data subclass is ambiguous (as is the case when matching an
	/// op_or element), then the caller is expected to use RTTI to determine
	/// the type of data being passed back.  The caller is responsible for
	/// calling delete on the pointer.
	virtual data* match(std::istream &parse_stream) const = 0;

	/// \brief Serialize given data into an ostream using this element tree
	///
	/// \param [in] tree_to_streamify
	/// Data tree to serialize via this grammar.  The tree will not be modified
	/// except possibly to cache string representations if the subclass feels
	/// like implementing it.
	///
	/// \return
	/// Pointer to a stringstream (NULL in case of failure, though every effort
	/// should be made to make sure that the data subclasses reject invalid
	/// data when it is put in, rather than waiting to fail on a call to
	/// streamify).  The caller is responsible for calling delete on the
	/// stringstream.
	

	/// \brief Abstract base class for parsed data
	///
	/// Abstract base class for parsed data.  Subclasses of data are welcome to
	/// have all the non-const members they want, because each thread or
	/// parsing operation has its own data tree.  Once a stream has been parsed
	/// into a data tree, the user may inspect it, modify it, and write it
	/// back to a stream.
	class data
	{
	public:

		/// \brief Pure virtual destructor.
		///
		/// Pure virtual destructor.  It only exists to allow proper virtual
		/// function table dispatching to the correct subclass destructor.
		virtual ~data() = 0;

		/// \brief Serialize data according to given grammarspec
		///
		/// \param [in] grammar
		/// Grammar with which to attempt to serialize this data.  This can be
		/// different from the grammar with which this data is read.  For
		/// example, one could parse a list of numbers with a grammar that
		/// specifies a python list format and serialize the list with a
		/// grammar that specifies a csv list format.
		/// 
		/// \return
		/// Pointer to a stringstream representation of the data (NULL in case
		/// this data cannot be serialized according to the given grammar,
		/// subject to two requirements:
		/// -# If we are currently in the process of getting data (somewhere
		///    within a tree of match() calls), then it should contain the
		///    original characters of the stream being parsed.  This is because
		///    we want the content of the stream to be unchanged in the case of
		///    a match failure.
		/// -# It should adhere to the implicit_whitespace setting of its
		///    corresponding element tree, within reason.
		virtual std::stringstream* streamify(const element &grammar) const = 0;


		const virtual void* get_datarep() const = 0;;
	};
};

// //Element to match arg1 AND arg2
// class op_and : element
// {
// public:
// 	and(const element &in_arg1, const element &in_arg2)
// 	:
// 		arg1(in_arg1),
// 		arg2(in_arg2)
// 	{
		
// 	}

// 	virtual results::vector* match(const std::istream &parse_stream) const
// 	{
		
// 	}

// private:
// 	const element arg1;
// 	const element arg2;
// };

class op_morethan : public element
{
public:
	morethan(const element &in_element_to_match, int n=0);

	virtual std::list<results::result*>* match(
		      std::istream &parse_stream,
	    const bool          ignore_whitespace = true
		) const;

private:
	const element element_to_match;
	const int n;
};

//match up to n characters from chars_to_match
//if n==-1, match any number of chars
class charset : public element
{
public:
	charset(const std::set<char> &in_chars_to_match, const int in_n = -1);
	charset(const std::string &in_string, const int in_n = -1);

	virtual std::list<results::result*>* match(
		      std::istream &parse_stream,
	    const bool          ignore_whitespace = true
	    ) const;
private:
	const std::set<char> chars_to_match;
	const int n;
};

class whitespace : public charset
{
public:
	whitespace();
};

class literal : public element
{
public:
	literal(const std::string &in_literal_to_match);

	//If the token is matched, returns a std::list of length 1, containing a
	//pointer to a results::token object containing the token.  If a token is
	//not matched, returns an empty std::list.
	virtual std::list<results::result*>* match(
		      std::istream &parse_stream,
	    const bool          ignore_whitespace = true
	    ) const;

private:
	const std::string literal_to_match;
};


}

#endif
