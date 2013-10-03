#include "acsetup.hpp"
#include "fastcgi-daemon/details/null_logger.hpp"

namespace fcgid { namespace details {

null_logger::null_logger()
{
}

null_logger::~null_logger() {
}

void
null_logger::info(char const *format, ...) {
	(void) format;
}

void
null_logger::debug(char const *format, ...) {
	(void) format;
}

void
null_logger::error(char const *format, ...) {
	(void) format;
}

}} // namespaces
