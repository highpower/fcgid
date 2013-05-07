#include "acsetup.hpp"
#include "fastcgi-daemon/details/fatal_error.hpp"

namespace fcgid { namespace details {

fatal_error::fatal_error()
{
}

char const*
fatal_error::what() const throw () {
	return "fatal error";
}

}} // namespaces
