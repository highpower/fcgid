// fastcgi-daemon is an advanced wrapper around libfcgi library written in c++
// Copyright (C) 2013 Rambler <highpower@rambler.ru>

// fastcgi-daemon is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef FASTCGI_DAEMON_DETAILS_CONTEXT_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_CONTEXT_HPP_INCLUDED

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/forward.hpp"
#include "fastcgi-daemon/details/range.hpp"

namespace fcgid { namespace details {

template <typename IO, typename Request, typename Response>
class context {

public:
	typedef IO io_type;
	typedef Request request_type;
	typedef Response response_type;
	typedef typename io_type::descriptor_type descriptor_type;

	context(descriptor_type const &d);

	io_type& io();
	response_type& response();
	request_type const& request() const;

	void accept();
	void finish();
	void status(http_status const &st);

private:
	io_type io_;
	request_type request_;
	response_type response_;
};

template <typename IO, typename Request, typename Response> inline
context<IO, Request, Response>::context(typename context<IO, Request, Response>::descriptor_type const &desc) :
	io_(desc), request_(io_), response_(io_)
{

}

template <typename IO, typename Request, typename Response> inline typename context<IO, Request, Response>::io_type& 
context<IO, Request, Response>::io() {
	return io_;
}

template <typename IO, typename Request, typename Response> inline typename context<IO, Request, Response>::response_type&
context<IO, Request, Response>::response() {
	return response_;
}

template <typename IO, typename Request, typename Response> inline typename context<IO, Request, Response>::request_type const&
context<IO, Request, Response>::request() const {
	return request_;
}

template <typename IO, typename Request, typename Response> inline void
context<IO, Request, Response>::accept() {
	io_.accept();
	request_.parse();
}

template <typename IO, typename Request, typename Response> inline void
context<IO, Request, Response>::finish() {
	io_.finish();
}

template <typename IO, typename Request, typename Response> inline void
context<IO, Request, Response>::status(http_status const &st) {
	io_.status(st);
}
	
}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_CONTEXT_HPP_INCLUDED
