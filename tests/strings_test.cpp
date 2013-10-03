#include "acsetup.hpp"

#include <list>
#include <vector>
#include <string>

#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include "test_utils.hpp"
#include "fastcgi-daemon/details/range.hpp"
#include "fastcgi-daemon/details/strings.hpp"
#include "fastcgi-daemon/details/functors.hpp"

namespace fcgid { namespace tests {

BOOST_AUTO_TEST_SUITE(string_utils_test)

typedef boost::mpl::list<std::list<char>, std::vector<char>, details::range<char const*>, std::string> test_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(test_trim, Sequence, test_types) {
	using namespace details;
	Sequence value = as<Sequence>(""), result = trim(value);
	BOOST_CHECK_EQUAL("", make_string(result));
	BOOST_CHECK_EQUAL("test", make_string(trim(as<Sequence>("  test  "))));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_trim_left, Sequence, test_types) {
	using namespace details;
	BOOST_CHECK_EQUAL("test  ", make_string(trim_left(as<Sequence>("   test  "))));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_trim_right, Sequence, test_types) {
	using namespace details;
	BOOST_CHECK_EQUAL("  test", make_string(trim_right(as<Sequence>("  test  "))));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_truncate, Sequence, test_types) {
	using namespace details;
	BOOST_CHECK_EQUAL("is is a te", make_string(truncate(as<Sequence>("this is a test"), 2, 2)));
	BOOST_CHECK_EQUAL("this is a test", make_string(truncate(as<Sequence>("this is a test"), 0, 0)));
	BOOST_CHECK_EQUAL("this is a test", make_string(truncate(as<Sequence>("this is a test"), 0, 0)));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_truncate_left, Sequence, test_types) {
	using namespace details;
	BOOST_CHECK_EQUAL("is is a test", make_string(truncate_left(as<Sequence>("this is a test"), 2)));
	BOOST_CHECK_EQUAL("this is a test", make_string(truncate_left(as<Sequence>("this is a test"), 0)));
	BOOST_CHECK_EQUAL("", make_string(truncate_left(as<Sequence>("this is a test"), sizeof("this is a test"))));
	BOOST_CHECK_EQUAL("", make_string(truncate_left(as<Sequence>("this is a test"), sizeof("this is a test") + 1)));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_truncate_right, Sequence, test_types) {
	using namespace details;
	BOOST_CHECK_EQUAL("this is a te", make_string(truncate_right(as<Sequence>("this is a test"), 2)));
	BOOST_CHECK_EQUAL("this is a test", make_string(truncate_right(as<Sequence>("this is a test"), 0)));
	BOOST_CHECK_EQUAL("", make_string(truncate_right(as<Sequence>("this is a test"), sizeof("this is a test"))));
	BOOST_CHECK_EQUAL("", make_string(truncate_right(as<Sequence>("this is a test"), sizeof("this is a test") + 1)));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_split, Sequence, test_types) {

	using namespace details;
	
	Sequence head, tail = as<Sequence>("one two");
	BOOST_CHECK(split_once(tail, ' ', head, tail));
	BOOST_CHECK(!split_once(tail, ' ', head, tail));
	
	tail = as<Sequence>("best test best test best");
	while (!tail.empty()) {
		split_once(tail, ' ', head, tail);
		BOOST_CHECK_EQUAL(4, head.size());
	}
	tail = as<Sequence>("best test best test best");
	while (!tail.empty()) {
		split_once(tail, "test", head, tail);
		BOOST_CHECK_EQUAL("best", make_string(trim(head)));
		tail = trim(tail);
	}
	tail = as<Sequence>("best test best test best");
	while (!tail.empty()) {
		split_once(tail, make_range("test"), head, tail);
		BOOST_CHECK_EQUAL("best", make_string(trim(head)));
		tail = trim(tail);
	}
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_split_if, Sequence, test_types) {

	using namespace details;
	Sequence head, tail = as<Sequence>("test test test test");
	while (!tail.empty()) {
		split_if_once(tail, is_space<char>(), head, tail);
		BOOST_CHECK_EQUAL(4, head.size());
		BOOST_CHECK_EQUAL("test", make_string(head));
	}
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_split_first_of, Sequence, test_types) {
	
	using namespace details;
	Sequence head, tail = as<Sequence>("a&b;c&d;e&f");
	while (!tail.empty()) {
		split_first_of_once(tail, "&;", head, tail);
		BOOST_CHECK_EQUAL(1, head.size());
	}
	tail = as<Sequence>("a&b;c&d;e&f");
	while (!tail.empty()) {
		split_first_of_once(tail, make_range("&;"), head, tail);
		BOOST_CHECK_EQUAL(1, head.size());
	}
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_split_reversed, Sequence, test_types) {

	using namespace details;
	Sequence seq = as<Sequence>("test test test test");
	range<typename Sequence::const_reverse_iterator> head, tail(seq.rbegin(), seq.rend());
	
	while (!tail.empty()) {
		split_if_once(tail, is_space<char>(), head, tail);
		BOOST_CHECK_EQUAL(4, head.size());
		BOOST_CHECK_EQUAL("tset", make_string(head));
	}
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_starts_ends, Sequence, test_types) {
	
	using namespace details;
	
	BOOST_CHECK(starts_with(as<Sequence>("testing"), "test"));
	BOOST_CHECK(!starts_with(as<Sequence>("production"), "test"));
	BOOST_CHECK(starts_with("testing", as<Sequence>("test")));
	BOOST_CHECK(!starts_with("production", as<Sequence>("test")));
	BOOST_CHECK(starts_with(as<Sequence>("testing"), as<Sequence>("test")));
	BOOST_CHECK(!starts_with(as<Sequence>("production"), as<Sequence>("test")));

	BOOST_CHECK(ends_with(as<Sequence>("testing"), "ing"));
	BOOST_CHECK(!ends_with(as<Sequence>("production"), "ing"));
	BOOST_CHECK(ends_with("testing", as<Sequence>("ing")));
	BOOST_CHECK(!ends_with("production", as<Sequence>("ing")));
	BOOST_CHECK(ends_with(as<Sequence>("testing"), as<Sequence>("ing")));
	BOOST_CHECK(!ends_with(as<Sequence>("production"), as<Sequence>("ing")));
}

BOOST_AUTO_TEST_SUITE_END();

}} // namespaces
