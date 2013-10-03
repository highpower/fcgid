#include "acsetup.hpp"

#include <list>
#include <string>
#include <vector>

#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include "test_utils.hpp"
#include "fastcgi-daemon/details/range.hpp"
#include "fastcgi-daemon/details/line_end_filter.hpp"

namespace fcgid { namespace tests { 

BOOST_AUTO_TEST_SUITE(line_end_test)

typedef boost::mpl::list<std::vector<char>, std::list<char>, details::range<char const*>, std::string> test_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(test_empty, Sequence, test_types) {

	char const *res[] = { "", " ", "  " }; 
	char const *val[] = { "", "\r\n ", " \r\n " };
	
	typedef details::line_end_filter<typename Sequence::const_iterator> iterator_type;
	for (std::size_t i = 0; i < sizeof(val) / sizeof(char const*); ++i) {
		Sequence const seq = as<Sequence>(val[i]);
		std::string result = make_string(details::make_line_end_filtered(seq.begin(), seq.end()));
		BOOST_CHECK_EQUAL(res[i], result);
	}
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_forward, Sequence, test_types) {
	

	char const *res[] = { 
		"abcdef abcdef abcdef abcdef", 
		"abcdef abcdef\r\nabcdef abcdef",
		"abcdef abcdef  abcdef abcdef",
		"abcdef abcdef abcdef abcdef\r\n" };

	char const *val[] = {
		"abcdef abcdef\r\n abcdef abcdef", 
		"abcdef abcdef\r\nabcdef abcdef", 
		"abcdef abcdef\r\n \r\n abcdef abcdef",
		"abcdef abcdef abcdef abcdef\r\n"
	};

	for (std::size_t i = 0; i < sizeof(val) / sizeof(char const*); ++i) {
		Sequence seq = as<Sequence>(val[i]);
		std::string result = make_string(details::make_line_end_filtered(seq));
		BOOST_CHECK_EQUAL(res[i], result);
	}
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_reverse, Sequence, test_types) {


	char const *res[] = { 
		"fedcba fedcba fedcba fedcba", 
		"fedcba fedcba\n\rfedcba fedcba", 
		"fedcba fedcba  fedcba fedcba", 
		"\n\rfedcba fedcba fedcba fedcba" };

	char const *val[] = {
		"abcdef abcdef\r\n abcdef abcdef", 
		"abcdef abcdef\r\nabcdef abcdef", 
		"abcdef abcdef\r\n \r\n abcdef abcdef",
		"abcdef abcdef abcdef abcdef\r\n"
	};

	for (std::size_t i = 0; i < sizeof(val) / sizeof(char const*); ++i) {
		Sequence seq = as<Sequence>(val[i]);
		typedef details::line_end_filter<typename Sequence::iterator> filtered_type;
		typedef std::reverse_iterator<filtered_type> iterator_type;
		details::range<filtered_type> r = details::make_line_end_filtered(seq.begin(), seq.end());
		BOOST_CHECK_EQUAL(res[i], std::string(r.rbegin(), r.rend()));
	}
}

BOOST_AUTO_TEST_SUITE_END();

}} // namespaces
