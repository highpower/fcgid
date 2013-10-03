#include "acsetup.hpp"
#include "fastcgi-daemon/details/request_parser.hpp"

#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <typeinfo>

#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include "test_utils.hpp"

namespace fcgid { namespace tests {

BOOST_AUTO_TEST_SUITE(parser_test)

typedef boost::mpl::list<std::list<char>, std::vector<char>, details::range<char const*>, std::string> test_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(test_multipart_one_line, Sequence, test_types) {
	
	Sequence seq = as<Sequence>("Content-Disposition: form-data; name=\"field\"\r\n");
	details::multipart_header_parser<typename Sequence::iterator> parser(seq.begin(), seq.end());
	BOOST_CHECK_EQUAL(false, parser.is_file());
	BOOST_CHECK_EQUAL("field", make_string(parser.name()));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_multipart_multi_line, Sequence, test_types) {
	Sequence seq = as<Sequence>("Content-Disposition: form-data; name=\"xfile\"; filename=\"test.dat\"\r\nContent-Type: application/octet-stream\r\n");
	details::multipart_header_parser<typename Sequence::iterator> parser(seq.begin(), seq.end());
	BOOST_CHECK_EQUAL(true, parser.is_file());
	BOOST_CHECK_EQUAL("xfile", make_string(parser.name()));
	BOOST_CHECK_EQUAL("test.dat", make_string(parser.file_name()));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_multipart_multi_line_headers, Sequence, test_types) {
	Sequence seq = as<Sequence>("Content-Disposition: form-data; name=\"xfile\";\r\n filename=\"test.dat\"\r\nContent-Type: application/octet-stream\r\n");
	details::multipart_header_parser<typename Sequence::iterator> parser(seq.begin(), seq.end());
	BOOST_CHECK_EQUAL(true, parser.is_file());
	BOOST_CHECK_EQUAL("xfile", make_string(parser.name()));
	BOOST_CHECK_EQUAL("test.dat", make_string(parser.file_name()));
}

BOOST_AUTO_TEST_SUITE_END();

}} // namespaces
