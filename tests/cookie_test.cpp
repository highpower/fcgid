#include "acsetup.hpp"

#include <string>
#include <boost/test/unit_test.hpp>

#include "fastcgi-daemon/error.hpp"
#include "fastcgi-daemon/cookie.hpp"

namespace fcgid { namespace tests {

bool
accepts_empty_name_error(error const &e) {
	return std::string(e.what()) == "can not create cookie without name";
}

BOOST_AUTO_TEST_SUITE(cookie_test)

BOOST_AUTO_TEST_CASE(test_value) {
	cookie c("name", "value");
}

BOOST_AUTO_TEST_CASE(test_empty_name) {
	BOOST_CHECK_EXCEPTION(cookie("", "value"), error, accepts_empty_name_error);
}

BOOST_AUTO_TEST_SUITE_END();


}} // namespaces
