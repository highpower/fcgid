#include "acsetup.hpp"

#include <algorithm>

#include "fastcgi-daemon/statistics.hpp"
#include "fastcgi-daemon/details/statistics_impl.hpp"

namespace fcgid {

statistics::statistics(statistics::impl_type const &impl) :
	impl_(impl)
{
}

statistics::statistics(statistics const &other) :
	impl_(other.impl_)
{
}

statistics&
statistics::operator = (statistics const &other) {
	statistics copy(other);
	swap(copy);
	return *this;
}

void
statistics::swap(statistics &other) throw () {
	using std::swap;
	swap(impl_, other.impl_);
}

std::size_t
statistics::requests_handled() const {
	return static_cast<impl_type const&>(impl_).requests_handled();
}

std::size_t
statistics::exceptions_occured() const {
	return static_cast<impl_type const&>(impl_).exceptions_occured();
}

std::size_t
statistics::client_errors_occured() const {
	return static_cast<impl_type const&>(impl_).client_errors_occured();
}

std::size_t
statistics::server_errors_occured() const {
	return static_cast<impl_type const&>(impl_).server_errors_occured();
}

std::size_t
statistics::requests_accepted() const {
	return static_cast<impl_type const&>(impl_).requests_accepted();
}

std::size_t
statistics::requests_in_queue(queue_name_type name) const {
	return static_cast<impl_type const&>(impl_).requests_in_queue(name);
}

} // namespace
