#include "acsetup.hpp"

#include <list>
#include <vector>
#include <string>

#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include "test_utils.hpp"
#include "fastcgi-daemon/details/range.hpp"
#include "fastcgi-daemon/details/tokenizer.hpp"

namespace fcgid { namespace tests {

BOOST_AUTO_TEST_SUITE(tokenizer_test)

typedef boost::mpl::list<std::list<char>, std::vector<char>, details::range<char const*>, std::string> test_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(test_tokenize_if, Range, test_types) {

	using namespace details;

	char const* values[] = { "123", "456", "789", "012", "345" };

	typedef is_space<char> checker_type;
	typedef split_if_match<checker_type> matcher_type;
	typedef tokenizer<Range, matcher_type> tokenizer_type;
	
	tokenizer_type tok(as<Range>("123 456 789 012 345"), matcher_type(checker_type()));
	typename tokenizer_type::const_iterator i = tok.begin(), end = tok.end();
	
	std::size_t count = 0;
	for (; i != end; ++i, ++count) {
		BOOST_CHECK_EQUAL(values[count], make_string(*i));
	}
	BOOST_CHECK_EQUAL(sizeof(values) / sizeof(char const*), count);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_tokenize_equal, Range, test_types) {

	using namespace details;

	typedef split_if_equal<char> matcher_type;
	typedef tokenizer<Range, matcher_type> tokenizer_type;
	char const* values[] = { "123", "456", "789", "012", "345" };
	
	tokenizer_type tok(as<Range>("123 456 789 012 345"), matcher_type(' '));
	typename tokenizer_type::const_iterator i = tok.begin(), end = tok.end();
	
	std::size_t count = 0;
	for (; i != end; ++i, ++count) {
		BOOST_CHECK_EQUAL(values[count], make_string(*i));
	}
	BOOST_CHECK_EQUAL(sizeof(values) / sizeof(char const*), count);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_tokenize_first_of, Range, test_types) {

	using namespace details;

	typedef split_first_of<char> matcher_type;
	typedef tokenizer<Range, matcher_type> tokenizer_type;
	char const* values[] = { "123", "456", "789", "012", "345" };
	
	tokenizer_type tok(as<Range>("123 456 789 012 345"), matcher_type(" \t"));
	typename tokenizer_type::const_iterator i = tok.begin(), end = tok.end();
	
	std::size_t count = 0;
	for (; i != end; ++i, ++count) {
		BOOST_CHECK_EQUAL(values[count], make_string(*i));
	}
	BOOST_CHECK_EQUAL(sizeof(values) / sizeof(char const*), count);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_tokenize_reversed, Range, test_types) {

	using namespace details;
	
	typedef split_if_equal<char> matcher_type;
	typedef range<typename Range::const_reverse_iterator> range_type;
	typedef tokenizer<range_type, matcher_type> tokenizer_type;
	
	Range seq = as<Range>("test test test test test");
	tokenizer_type tok(range_type(seq.rbegin(), seq.rend()), matcher_type(' '));
	for (typename tokenizer_type::const_iterator i = tok.begin(), end = tok.end(); i != end; ++i) {
		BOOST_CHECK_EQUAL("tset", make_string(*i));
	}
}

BOOST_AUTO_TEST_SUITE_END();

}} // namespaces
