#include "acsetup.hpp"
#include "fastcgi-daemon/details/statistics_impl.cpp"

namespace fcgid { namespace details {

statistics_impl::statistics_impl()
{
}

statistics_impl::~statistics_impl() {
}

std::size_t
statistics_impl::requests_handled() const {
}

std::size_t
statistics_impl::exceptions_occured() const {
}

std::size_t
statistics_impl::client_errors_occured() const {
}

std::size_t
statistics_impl::server_errors_occured() const {
}

std::size_t
statistics_impl::requests_accepted() const {
}

std::size_t
statistics_impl::requests_in_queue(char const *name) const {
}

}} // namespaces
