#include "acsetup.hpp"

#include <list>
#include <vector>
#include <string>
#include <sstream>

#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include "test_utils.hpp"
#include "fastcgi-daemon/details/range.hpp"

namespace fcgid { namespace tests {

BOOST_AUTO_TEST_SUITE(range_test)

typedef boost::mpl::list<std::list<char>, std::vector<char>, details::range<char const*>, std::string> test_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(test_output, Sequence, test_types) {
	std::stringstream stream;
	Sequence seq = as<Sequence>("test string");
	details::range<typename Sequence::const_iterator> range(seq.begin(), seq.end());
	stream << range;
	BOOST_CHECK_EQUAL(make_string(seq), stream.str());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_reversed, Sequence, test_types) {

	Sequence value = as<Sequence>("123 456");
	details::range<typename Sequence::iterator> range(value.begin(), value.end());
	
	BOOST_CHECK_EQUAL(std::string("654 321"), std::string(range.rbegin(), range.rend()));
	BOOST_CHECK_EQUAL(std::string(value.rbegin(), value.rend()), std::string(range.rbegin(), range.rend()));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_construct, Sequence, test_types) {

	details::range<typename Sequence::const_iterator> range;
	BOOST_CHECK(range.empty());
	BOOST_CHECK_EQUAL(0, range.size());

	Sequence seq = as<Sequence>("12345");
	details::range<typename Sequence::const_iterator> other = details::make_range(seq);
	BOOST_CHECK(!other.empty());
	BOOST_CHECK_EQUAL(5, other.size());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_comparision, Sequence, test_types) {

	Sequence seq = as<Sequence>("test string");
	details::range<char const*> pattern = details::make_range("test string");
	details::range<typename Sequence::const_iterator> empty, range(seq.begin(), seq.end());

	BOOST_CHECK(empty < pattern);
	BOOST_CHECK(pattern > empty);
	BOOST_CHECK(range == pattern);
	BOOST_CHECK(range <= pattern);
	BOOST_CHECK(range >= pattern);
	
	BOOST_CHECK_EQUAL(false, range != pattern);
	BOOST_CHECK_EQUAL(false, empty == pattern);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_reversed_comparision, Sequence, test_types) {

	Sequence seq = as<Sequence>("test string");
	details::range<char const*> source = details::make_range("test string");
	details::range<typename Sequence::const_reverse_iterator> range(seq.rbegin(), seq.rend());
	details::range<details::range<char const*>::const_reverse_iterator> empty, pattern(source.rbegin(), source.rend());
	
	BOOST_CHECK(empty < pattern);
	BOOST_CHECK(pattern > empty);
	BOOST_CHECK(range == pattern);
	BOOST_CHECK(range <= pattern);
	BOOST_CHECK(range >= pattern);

	BOOST_CHECK_EQUAL(false, range != pattern);
	BOOST_CHECK_EQUAL(false, empty == pattern);
}

BOOST_AUTO_TEST_SUITE_END();

}} // namespaces
