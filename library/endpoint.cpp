#include "acsetup.hpp"
#include "fastcgi-daemon/endpoint.hpp"

#include <string>
#include <sstream>
#include <algorithm>

#include "fastcgi-daemon/details/endpoint_impl.hpp"

namespace fcgid {

endpoint::endpoint() :
	impl_()
{
}

endpoint::~endpoint() {
}

endpoint::endpoint(endpoint const &other) :
	impl_(other.impl_)
{
}

endpoint::endpoint(std::shared_ptr<impl_type> const &impl) :
	impl_(impl)
{
}

endpoint&
endpoint::operator = (endpoint const &other) {
	endpoint copy(other);
	swap(copy);
	return *this;
}

void
endpoint::swap(endpoint &other) throw () {
	using std::swap;
	swap(impl_, other.impl_);
}

bool
endpoint::operator == (endpoint const &other) const {
	if (impl_ && other.impl_) {
		return *impl_ == *other.impl_;
	}
	return false;
}

bool
endpoint::operator != (endpoint const &other) const {
	return !operator == (other);
}

endpoint
endpoint::create_unix_socket(char const *path, unsigned short backlog) {
	std::shared_ptr<impl_type> ep(new impl_type(path, backlog));
	return endpoint(ep);
}

endpoint
endpoint::create_netword_socket(char const *addr, unsigned short port, unsigned short backlog) {
	std::stringstream stream;
	stream << addr << ':' << port;
	std::string result = stream.str();
	std::shared_ptr<impl_type> ep(new impl_type(result.c_str(), backlog));
	return endpoint(ep);
}

std::shared_ptr<endpoint::impl_type> const&
endpoint::impl() const {
	return impl_;
}

} // namespace
