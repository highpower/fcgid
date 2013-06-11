#include "acsetup.hpp"
#include "fastcgi-daemon/details/statistics_impl.hpp"

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

void
statistics_impl::server_error_occured() {
}

std::size_t
statistics_impl::server_errors_occured() const {
}

void
statistics_impl::request_accepted() {
}

std::size_t
statistics_impl::requests_accepted() const {
}

std::size_t
statistics_impl::requests_in_queue(queue_name_type queue) const {
}

}} // namespaces
