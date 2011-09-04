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

	/// \brief Abstract base class for parsed data
	///
	/// Abstract base class for parsed data.  Subclasses of data are welcome to
	/// have all the non-const members they want, because each thread or
	/// parsing operation has its own data tree.  Once a stream has been parsed
	/// into a data tree, the user may inspect it, modify it, and write it
	/// back to a stream.
	/// 
	/// If a given piece of data is meant to be an aggregate, then it must
	/// directly support that aggregate's interface.  For example, an
	/// op_morethan IS-A std::list of data pointers.  Data elements
	/// corresponding to base types (int, double, etc.) may show different
	/// semantics.
	///
	/// Most subclasses of data will have two different constructors:
	/// -# One to initialize from a string being parsed
	/// -# One from the correct sort of program data, e.g. a 
	///    grammarspec::double::data from a double.
	class data
	{
	public:

		/// \brief Pure virtual destructor.
		///
		/// Pure virtual destructor.  It only exists to allow proper virtual
		/// function table dispatching to the correct subclass destructor.
		virtual ~data() = 0;

		/// \brief Serialize data according to given grammarspec. 
		///
		/// \param [in] grammar
		/// Grammar with which to attempt to serialize this data.  This can be
		/// different from the grammar with which this data is read.  For
		/// example, one could parse a list of numbers with a grammar that
		/// specifies a python list format and serialize the list with a
		/// grammar that specifies a csv list format.  THIS ARGUMENT IS IGNORED
		/// FOR NOW.
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
		/// The caller is responsible for calling delete on the stringstream
		virtual std::stringstream* streamify(const element &grammar) const = 0;

		/// \brief Serialize the data directly to a given stream.
		///
		/// \param [in] grammar
		/// Grammar to use when attempting to serialize the data.  This
		/// argument is ignored for now.
		/// 
		/// \param [in,out] outstream
		/// Stream target for serialization.  If the serialization fails, then
		/// the stream is unmodified.
		/// 
		/// \exception grammarspec::element::data::serialization_failure
		/// Thrown in if the data cannot be serialized by the given grammar.
		virtual void serialize_to_stream(const element      &grammar,
		                                       std::ostream &outstream
		                                 ) const = 0;
		
		/// \brief Unwind the data back onto the given istream.
		///
		/// Serialize this data and place it in reverse onto the head of the
		/// specified istream.  This operation is useful in the case of match
		/// failure at a higher level in the grammarspec tree.
		///
		/// \param [in] grammar
		/// Grammar to use when attempting to serialize the data.  This
		/// argument is ignored for now.
		///
		/// \param [in,out] instream
		/// Stream target for serialization.  If the serialization fails, then
		/// the stream is unmodified.
		/// 
		/// \exception grammarspec::element::data::serialization_failure
		/// Thrown in if the data cannot be serialized by the given grammar.
		virtual void unwind_to_stream(const element      &grammar,
		                                    std::istream &instream
		                              ) const = 0;
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

// class op_morethan : public element
// {
// public:
// 	morethan(const element &in_element_to_match, int n=0);

// 	virtual std::list<results::result*>* match(
// 		      std::istream &parse_stream,
// 	    const bool          ignore_whitespace = true
// 		) const;

// private:
// 	const element element_to_match;
// 	const int n;
// };

// //match up to n characters from chars_to_match
// //if n==-1, match any number of chars
// class charset : public element
// {
// public:
// 	charset(const std::set<char> &in_chars_to_match, const int in_n = -1);
// 	charset(const std::string &in_string, const int in_n = -1);

// 	virtual std::list<results::result*>* match(
// 		      std::istream &parse_stream,
// 	    const bool          ignore_whitespace = true
// 	    ) const;
// private:
// 	const std::set<char> chars_to_match;
// 	const int n;
// };

// class whitespace : public charset
// {
// public:
// 	whitespace();
// };

/// \brief A parse element that matches a string literal
///
/// A parse element that matched a string literal.  Because literals do not
/// often contain interesting data, a given grammar specification will
/// probably want to wrap literals in an op_noreport to avoid cluttering the
/// data that is returned.
///
/// At least for now, literal is a parse atom (a leaf of any parse tree that
/// it is in).  In theory, it could be made up of individual character atoms,
/// but that's a bit ridiculous.  The (very small) downside of having a string
/// literal be a parse atom is that the range of grammars used to serialize a
/// literal will only be able to wrap things around the literal, not modify
/// the literal itself by (for example) spacing out the individual characters.
class literal : public element
{
public:

	/// \brief Construct a literal that matches the given string.
	///
	/// \param [in] in_literal_to_match
	/// The string that this literal parsing element should match against.
	literal(const std::string &in_literal_to_match);

	/// \brief Attempt to match this literal from the head of the given stream.
	///
	/// \param [in,out] parse_stream
	/// Stream to match this literal from.  If the match succeeds, the matching
	/// characters will be removed from the head of the stream.  If the match
	/// fails, there will be no change in the characters of the stream.
	///
	/// \return
	/// A pointer to a grammarspec::literal::data object, which can be used as
	/// a pointer to a std::string object.  The caller is responsible for
	/// calling delete on the data.  NULL is returned if the match fails.
	virtual data* match(std::istream &parse_stream) const;

private:

	const std::string literal_to_match;

public:

	/// \brief A data type corresponding to a literal parse element.
	///
	/// A data type corresponding to a literal parse element.  It IS-A string.
	class data
	:
		public element::data,
		public std::string
	{
	public:

		/// \brief Deinitialize this data object.
		///
		/// Deinitialize this data object.  Doesn't need to do anything except
		/// properly dispatch to the std::string destructor.
		virtual ~data();

		/// \brief Serialize this literal::data object into a stream.
		///
		/// Serialize this data object into a stream.  See
		/// \ref element::data::streamify(const element &grammar) const
		/// for more info.
		///
		/// \param [in] grammar
		/// Eventually, this will be any grammar that contains one
		/// grammarspec::literal with the same string as this data.  This will
		/// be useful, if, for example, one wants to surround all literals with
		/// quotes.  For now, it's ignored, and this literal is simply directly
		/// serialized
		virtual std::stringstream* streamify(const element &grammar) const = 0;

		/// \brief Serialize this literal::data directly to an ostream.
		///
		/// Serialize this literal::data directly to an ostream.  See
		/// \ref element::data::serialize_to_stream() and
		/// \ref literal::data::streamify()
		/// for more info on return types and behavior of parameters.
		virtual void serialize_to_stream(const element      &grammar,
		                                       std::ostream &outstream
		                                 ) const = 0;

		/// \brief Unwind this literal::data back on to the given istream.
		///
		/// Unwind this literal::data back on to the given istream.  See
		/// \ref element::data::unwind_to_stream() and
		/// \ref literal::data::streamify()
		/// for more info on return types and behavior of parameters.
		virtual void unwind_to_stream(const element      &grammar,
		                                    std::istream &instream
		                              ) const = 0;
	};
};


}

#endif
