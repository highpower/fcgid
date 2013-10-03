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

#ifndef FASTCGI_DAEMON_DETAILS_STANDARD_RESPONSE_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_STANDARD_RESPONSE_HPP_INCLUDED

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/cookie.hpp"
#include "fastcgi-daemon/response.hpp"
#include "fastcgi-daemon/details/generic_response.hpp"

namespace fcgid { namespace details {

template <typename IO>
class standard_response : public response, public generic_response<IO, stdalloc> {

public:
	typedef IO io_type;
	typedef standard_response<IO> type;
	typedef generic_response<IO, stdalloc> base_type;

	standard_response(IO &io);
	virtual ~standard_response();

	virtual void status(http_status const &st);
	virtual void header(std::string const &name, std::string const &value);
	
	virtual void redirect(std::string const &url);
	virtual void expire_time(http_date const &exp);
	virtual void expire_time(std::string const &exp);
	
	virtual void set_cookie(cookie const &c);
	virtual std::size_t write(char const *buffer, std::size_t size);
	
private:	
	standard_response(standard_response const &);
	standard_response& operator = (standard_response const &);
};

template <typename IO> inline 
standard_response<IO>::standard_response(IO &io) :
	base_type(io)
{
}

template <typename IO> inline 
standard_response<IO>::~standard_response() {
}

template <typename IO> inline void
standard_response<IO>::status(http_status const &st) {
	base_type::status(st);
}

template <typename IO> inline void
standard_response<IO>::header(std::string const &name, std::string const &value) {
	base_type::header(name, value);
}

template <typename IO> inline void
standard_response<IO>::redirect(std::string const &url) {
	base_type::redirect(url);
}

template <typename IO> inline void
standard_response<IO>::expire_time(http_date const &exp) {
	base_type::expire_time(exp);
}

template <typename IO> inline void
standard_response<IO>::expire_time(std::string const &exp) {
	base_type::expire_time(exp);
}

template <typename IO> inline void
standard_response<IO>::set_cookie(cookie const &c) {
	base_type::set_cookie(c.impl());
}

template <typename IO> inline std::size_t
standard_response<IO>::write(char const *buffer, std::size_t size) {
	return base_type::write(buffer, size);
}

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_STANDARD_RESPONSE_HPP_INCLUDED
