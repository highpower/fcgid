#include "acsetup.hpp"
#include "fastcgi-daemon/details/runtime_error.hpp"

#include <cstdio>
#include <cstdarg>

namespace fcgid { namespace details {

runtime_error::runtime_error(char const *format, ...) {
	va_list args;
	va_start(args, format);
	vsnprintf(message_, message_size, format, args);
	va_end(args);
}

char const*
runtime_error::what() const throw () {
	return message_;
}

}} // namespaces
