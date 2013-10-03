#include "acsetup.hpp"
#include "fastcgi-daemon/details/server_impl.hpp"

namespace fcgid { namespace details {

server_impl::server_impl()
{
}

server_impl::~server_impl() {
}

server_impl::invoker_type&
server_impl::invoker() {
	matcher_type &m = matcher();
	return m.invoker();
}

}} // namespaces
