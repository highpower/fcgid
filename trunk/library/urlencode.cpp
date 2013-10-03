#include "acsetup.hpp"
#include "fastcgi-daemon/urlencode.hpp"

#include "fastcgi-daemon/details/range.hpp"
#include "fastcgi-daemon/details/urlencode_impl.hpp"

namespace fcgid {

std::string
urlencode(char const *value) {
	return details::urlencode<std::string>(details::make_range(value));
}

std::string
urlencode(std::string const &value) {
	return details::urlencode<std::string>(value);
}

std::string
urldecode(char const *value) {
	return details::urldecode<std::string>(details::make_range(value));
}

std::string
urldecode(std::string const &value) {
	return details::urldecode<std::string>(value);
}

} // namespace
