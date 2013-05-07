#include "acsetup.hpp"
#include "fastcgi-daemon/http_method.hpp"

#include <boost/concept_check.hpp>
#include <boost/test/unit_test.hpp>

namespace fcgid { namespace tests {

BOOST_CONCEPT_ASSERT((boost::Assignable<http_method>));
BOOST_CONCEPT_ASSERT((boost::CopyConstructible<http_method>));
BOOST_CONCEPT_ASSERT((boost::EqualityComparable<http_method>));

BOOST_AUTO_TEST_SUITE(http_method_test)

BOOST_AUTO_TEST_CASE(test_parse) {
	http_method get = http_method::parse("GET");
	BOOST_CHECK_EQUAL(get, http_method::get);
	BOOST_CHECK_EQUAL(true, get == http_method::get);
	
	http_method head = http_method::parse("HEAD");
	BOOST_CHECK_EQUAL(head, http_method::head);
	BOOST_CHECK_EQUAL(true, head == http_method::head);

	http_method post = http_method::parse("POST");
	BOOST_CHECK_EQUAL(post, http_method::post);
	BOOST_CHECK_EQUAL(true, post == http_method::post);
	
	http_method unknown = http_method::parse("UNKNOWN");
	BOOST_CHECK_EQUAL(unknown, http_method::unknown);
	BOOST_CHECK_EQUAL(true, unknown == http_method::unknown);

	BOOST_CHECK_EQUAL(http_method::parse("GET"), http_method::parse("get"));
	BOOST_CHECK_EQUAL(http_method::parse("HEAD"), http_method::parse("head"));
	BOOST_CHECK_EQUAL(http_method::parse("POST"), http_method::parse("post"));
}

BOOST_AUTO_TEST_CASE(test_http_mask) {
	
	BOOST_CHECK(http_method_mask::all.accepts(http_method::get));
	BOOST_CHECK(http_method_mask::get_only.accepts(http_method::get));
	BOOST_CHECK(!http_method_mask::post_only.accepts(http_method::get));
	
	BOOST_CHECK(http_method_mask::all.accepts(http_method::post));
	BOOST_CHECK(http_method_mask::post_only.accepts(http_method::post));
	BOOST_CHECK(!http_method_mask::get_only.accepts(http_method::post));
	
	
	BOOST_CHECK(http_method_mask::get_only.accepts(http_method::head));
	BOOST_CHECK(http_method_mask::post_only.accepts(http_method::head));
}


BOOST_AUTO_TEST_SUITE_END();

}} // namespaces
