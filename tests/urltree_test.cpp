#include "acsetup.hpp"

#include <boost/test/unit_test.hpp>

#include "mock_request.hpp"
#include "mock_response.hpp"

#include "fastcgi-daemon/http_error.hpp"
#include "fastcgi-daemon/http_status.hpp"
#include "fastcgi-daemon/details/urltree.hpp"

namespace fcgid { namespace tests {

bool
is_not_found(http_error const &e) {
	return e.status() == http_status::not_found;
}

BOOST_AUTO_TEST_SUITE(urltree_test)

BOOST_AUTO_TEST_CASE(test_existent) {
}

BOOST_AUTO_TEST_CASE(test_wildcard) {
}

BOOST_AUTO_TEST_CASE(test_existent_wildcard) {
}

BOOST_AUTO_TEST_CASE(test_nonexistent) {
}

BOOST_AUTO_TEST_SUITE_END();

}} // namespaces
