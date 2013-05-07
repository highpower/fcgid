#include "acsetup.hpp"
#include "fastcgi-daemon/details/endpoint_impl.hpp"
#include "fastcgi-daemon/details/runtime_error.hpp"

namespace fcgid { namespace details {

endpoint_impl::endpoint_impl(char const *addr, unsigned short backlog) :
	socket_(-1), addr_(addr)
{
	socket_ = FCGX_OpenSocket(addr, backlog);
	if (-1 == socket_) {
		throw runtime_error("can not open socket on %s", addr_.c_str());
	}
}

endpoint_impl::~endpoint_impl() {
	
}

int 
endpoint_impl::socket() const {
	return socket_;
}

bool
endpoint_impl::operator == (endpoint_impl const &other) const {
	return addr_ == other.addr_;
}

}} // namespaces
