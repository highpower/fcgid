#include "acsetup.hpp"
#include "test_utils.hpp"

#include <iostream>
#include <iterator>
#include <boost/functional/hash.hpp>

namespace fcgid { namespace tests {

std::size_t
hash_code(std::istream &is) {
	is.clear();
	is.seekg(0);
	return boost::hash_range(std::istream_iterator<char>(is), std::istream_iterator<char>());
}

}} // namespaces
