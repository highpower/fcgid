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

FCGX_Request*
fastcgi_request_traits::default_value() {
	return static_cast<FCGX_Request*>(0);
}

void
fastcgi_request_traits::destroy(FCGX_Request *req) {
	FCGX_Free(req, 1);
}

fastcgi::fastcgi(endpoint const &desc)
{
	boost::call_once(&fastcgi::init_fastcgi_engine, fastcgi_init_flag);
	boost::shared_ptr<endpoint_impl> const &impl = desc.impl();
	
	req_.reset(new FCGX_Request());
	memset(req_.get(), 0, sizeof(FCGX_Request));
	if (FCGX_InitRequest(req_.get(), impl->socket(), 0) != 0) {
		throw fatal_error();
	}
}

fastcgi::~fastcgi() {
}

void
fastcgi::accept() {
	assert(!accepted());
	if (0 != FCGX_Accept_r(req_.get())) {
		throw fatal_error();
	}
}

void
fastcgi::finish() {
	if (accepted()) {
		FCGX_Finish_r(req_.get());
	}
}

char const* const*
fastcgi::environ() const {
	assert(accepted());
	return req_.get()->envp;
}

std::size_t
fastcgi::read(char *buffer, std::size_t size) {
	assert(accepted());
	return static_cast<std::size_t>(FCGX_GetStr(buffer, size, req_.get()->in));
}

bool
fastcgi::accepted() const {
	return req_.get()->requestId != 0;
}

void
fastcgi::status(http_status const &st) {
	(void) st;
}

std::size_t
fastcgi::write(char const *buffer, std::size_t size) {
	assert(accepted());
	return static_cast<std::size_t>(FCGX_PutStr(buffer, size, req_.get()->out));
}

std::size_t
fastcgi::write_header(range<char const*> const &name, range<char const*> const &value) {
	return 0;
}

void
fastcgi::init_fastcgi_engine() {
	FCGX_Init();
}

}} // namespaces
