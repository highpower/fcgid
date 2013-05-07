#include "acsetup.hpp"

#include <list>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iterator>

#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/functional/hash.hpp>

#include "test_utils.hpp"
#include "fastcgi-daemon/details/range.hpp"
#include "fastcgi-daemon/details/iter_streambuf.hpp"

namespace fcgid { namespace tests {

BOOST_AUTO_TEST_SUITE(iter_streambuf_test)

typedef boost::mpl::list<std::list<char>, std::vector<char>, std::string> mutable_test_types;
typedef boost::mpl::list<std::list<char>, std::vector<char>, details::range<char const*>, std::string> test_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(test_getline, Sequence, test_types) {

	char source[] = "123 456\n789 012\n345 678\n901 234\n567 890";
	Sequence seq = as<Sequence>(source);
	details::iter_streambuf<typename Sequence::const_iterator> buffer(seq.begin(), seq.end());
	
	std::string line;
	std::istream stream(&buffer);
	
	std::size_t i = 0;
	char const* values[] = { "123 456", "789 012", "345 678", "901 234", "567 890" };
	
	BOOST_CHECK(stream.good());
	for (; std::getline(stream, line); ++i) {
		BOOST_CHECK_EQUAL(values[i], line);
	}
	BOOST_CHECK(stream.eof());
	BOOST_CHECK_EQUAL(sizeof(values) / sizeof(char const*), i);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_putback, Sequence, test_types) {

	char source[] = "123 456 012 345 678 901 234 567 890";
	Sequence seq = as<Sequence>(source);

	char value[8]; // sizeof("123 456")
	details::iter_streambuf<typename Sequence::const_iterator> buffer(seq.begin(), seq.end());
	
	std::istream stream(&buffer);
	BOOST_CHECK(stream.good());
	
	stream.putback('#');
	BOOST_CHECK(stream.fail());
	stream.clear();
	
	memset(value, 0, sizeof(value));
	stream.get(value, sizeof(value));

	BOOST_CHECK(stream.good());
	BOOST_CHECK_EQUAL(sizeof(value) - 1, stream.gcount());
	BOOST_CHECK_EQUAL("123 456", std::string(value));
	
	for (std::size_t i = 0; i < sizeof(value) - 1; ++i) {
		stream.unget();
		BOOST_CHECK(stream.good());
	}

	memset(value, 0, sizeof(value));
	stream.get(value, sizeof(value));

	BOOST_CHECK(stream.good());
	BOOST_CHECK_EQUAL(sizeof(value) - 1, stream.gcount());
	BOOST_CHECK_EQUAL("123 456", std::string(value));
}


BOOST_AUTO_TEST_CASE_TEMPLATE(test_position, Sequence, test_types) {

	char source[] = "123 456 012 345 678 901 234 567 890";
	Sequence seq = as<Sequence>(source);
	
	char value[8]; // sizeof "123 456"
	details::iter_streambuf<typename Sequence::const_iterator> buffer(seq.begin(), seq.end());
	
	std::istream stream(&buffer);
	stream.seekg(static_cast<std::streamoff>(8), std::ios::cur);
	BOOST_CHECK(stream.good());

	stream.get(value, sizeof(value));
	BOOST_CHECK(stream.good());
	BOOST_CHECK_EQUAL(sizeof(value) - 1, stream.gcount());
	BOOST_CHECK_EQUAL("012 345", std::string(value));
	BOOST_CHECK_EQUAL(8 + sizeof(value) - 1, stream.tellg());
	
	stream.seekg(0);
	BOOST_CHECK(stream.good());
	BOOST_CHECK_EQUAL(static_cast<std::streampos>(0), stream.tellg());
	
	stream.get(value, sizeof(value));
	BOOST_CHECK_EQUAL(sizeof(value) - 1, stream.gcount());
	BOOST_CHECK_EQUAL("123 456", std::string(value));
	
	stream.seekg(static_cast<std::streamoff>(-2), std::ios::cur);
	BOOST_CHECK(stream.good());
	BOOST_CHECK_EQUAL(sizeof(value) - 3, stream.tellg());

	stream.seekg(static_cast<std::streamoff>(-2), std::ios::beg);
	BOOST_CHECK(stream.fail());
	stream.clear();
}


	
BOOST_AUTO_TEST_CASE_TEMPLATE(test_checksum, Sequence, mutable_test_types) {

	std::ifstream file;
	file.exceptions(std::ios::badbit);
	file.open("test-data/install.dat", std::ios::binary);
	
	Sequence seq;
	std::copy(std::istream_iterator<char>(file), std::istream_iterator<char>(), std::back_inserter(seq));
	std::size_t target = boost::hash_range(seq.begin(), seq.end());

	typedef details::iter_streambuf<typename Sequence::const_iterator> streambuf_type;
	streambuf_type buffer(seq.begin(), seq.end());
	std::istream stream(&buffer);
	
	BOOST_CHECK_EQUAL(target, hash_code(file));
	BOOST_CHECK_EQUAL(target, hash_code(stream));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_rdbuf, Sequence, mutable_test_types) {

	Sequence seq;
	std::ifstream file;
	std::stringstream target, stream;
	
	file.exceptions(std::ios::badbit);
	file.open("test-data/install.dat", std::ios::binary);
	file >> std::noskipws;
	std::copy(std::istream_iterator<char>(file), std::istream_iterator<char>(), std::back_inserter(seq));
	file.clear();
	file.seekg(0);
	
	typedef details::iter_streambuf<typename Sequence::const_iterator> streambuf_type;
	streambuf_type buffer(seq.begin(), seq.end());

	target << file.rdbuf();
	stream << &buffer;
	
	BOOST_CHECK_EQUAL(target.str(), stream.str());
}

BOOST_AUTO_TEST_SUITE_END();

}} // namespaces
