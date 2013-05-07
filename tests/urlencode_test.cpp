#include "acsetup.hpp"

#include <list>
#include <vector>
#include <string>

#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include "test_utils.hpp"
#include "fastcgi-daemon/details/range.hpp"
#include "fastcgi-daemon/details/urlencode_impl.hpp"

namespace fcgid { namespace tests {

BOOST_AUTO_TEST_SUITE(urlencode_test)

typedef boost::mpl::list<std::list<char>, std::vector<char>, details::range<char const*>, std::string> test_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(test_urlencode, Range, test_types) {
	Range value = as<Range>("раз два три четыре пять");
	BOOST_CHECK_EQUAL("%D2%C1%DA%20%C4%D7%C1%20%D4%D2%C9%20%DE%C5%D4%D9%D2%C5%20%D0%D1%D4%D8", details::urlencode<std::string>(value));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_urlencode_empty, Range, test_types) {
	BOOST_CHECK_EQUAL("", details::urlencode<std::string>(Range()));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_urlencode_latin, Range, test_types) {
	Range value = as<Range>("abcd efgh");
	BOOST_CHECK_EQUAL("abcd%20efgh", details::urlencode<std::string>(value));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_urldecode, Range, test_types) {
	Range value = as<Range>("%D2%C1%DA%20%C4%D7%C1%20%D4%D2%C9%20%DE%C5%D4%D9%D2%C5%20%D0%D1%D4%D8");
	BOOST_CHECK_EQUAL("раз два три четыре пять", details::urldecode<std::string>(value));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_urldecode_empty, Range, test_types) {
	BOOST_CHECK_EQUAL("", details::urldecode<std::string>(Range()));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_urldecode_latin, Range, test_types) {
	Range value = as<Range>("abcd%20efgh");
	BOOST_CHECK_EQUAL("abcd efgh", details::urldecode<std::string>(value));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_urldecode_bad_suffix, Range, test_types) {
	Range value = as<Range>("abcd%20efgh%");
	BOOST_CHECK_EQUAL("abcd efgh%", details::urldecode<std::string>(value));
}

BOOST_AUTO_TEST_SUITE_END()

}} // namespaces
