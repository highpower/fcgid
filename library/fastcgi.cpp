#include "acsetup.hpp"
#include "fastcgi-daemon/details/fastcgi.hpp"

#include <cassert>
#include <cstring>
#include <boost/thread.hpp>

#include "fastcgi-daemon/endpoint.hpp"
#include "fastcgi-daemon/http_status.hpp"
#include "fastcgi-daemon/details/fatal_error.hpp"
#include "fastcgi-daemon/details/endpoint_impl.hpp"

namespace fcgid { namespace details {

boost::once_flag fastcgi_init_flag;

fastcgi::fastcgi(endpoint const &desc)
{
	boost::call_once(&fastcgi::init_fastcgi_engine, fastcgi_init_flag);
	boost::shared_ptr<endpoint_impl> const &impl = desc.impl();
	memset(&req_, 0, sizeof(FCGX_Request));
	if (FCGX_InitRequest(&req_, impl->socket(), 0) != 0) {
		throw fatal_error();
	}
}

fastcgi::~fastcgi() {
	FCGX_Free(&req_, 1);
}

void
fastcgi::accept() {
	assert(!is_accepted());
	if (0 != FCGX_Accept_r(&req_)) {
		throw fatal_error();
	}
}

void
fastcgi::finish() {
	if (is_accepted()) {
		FCGX_Finish_r(&req_);
	}
}

char const* const*
fastcgi::environ() const {
	assert(is_accepted());
	return req_.envp;
}

std::size_t
fastcgi::read(char *buffer, std::size_t size) {
	assert(is_accepted());
	return static_cast<std::size_t>(FCGX_GetStr(buffer, size, req_.in));
}

bool
fastcgi::is_accepted() const {
	return req_.requestId != 0;
}

void
fastcgi::status(http_status const &st) {
	(void) st;
}

void
fastcgi::init_fastcgi_engine() {
	FCGX_Init();
}

}} // namespaces
