#include "acsetup.hpp"

#include <fstream>

#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include "mock_io.hpp"
#include "test_utils.hpp"
#include "fastcgi-daemon/details/standard_response.hpp"

namespace fcgid { namespace tests {

bool
accepts_bad_status_error(error const &) {
	return true;
}

typedef boost::mpl::list<std::list<char>, std::vector<char>, details::range<char const*>, std::string> test_types;

BOOST_AUTO_TEST_SUITE(response_test)

BOOST_AUTO_TEST_CASE_TEMPLATE(test_parse_status, Range, test_types) {
	
	Range ok = as<Range>("200 OK");
	BOOST_CHECK(http_status::ok == (details::parse_status(ok.begin(), ok.end())));
	
	Range other = as<Range>("200");
	BOOST_CHECK(http_status::ok == (details::parse_status(other.begin(), other.end())));

	Range bad = as<Range>("bad status");
	BOOST_CHECK_EXCEPTION((details::parse_status(bad.begin(), bad.end())), error, accepts_bad_status_error);
}



BOOST_AUTO_TEST_CASE(test_send_headers) {
}

BOOST_AUTO_TEST_CASE(test_write_no_content) {
}

BOOST_AUTO_TEST_SUITE_END();

}} // namespaces

