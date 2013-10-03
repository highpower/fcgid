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

#ifndef FASTCGI_DAEMON_RESPONSE_HPP_INCLUDED
#define FASTCGI_DAEMON_RESPONSE_HPP_INCLUDED

#include <string>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/forward.hpp"

namespace fcgid {

class FASTCGI_DAEMON_API response {

public:
	response();
	virtual ~response();
	
	virtual void status(http_status const &st) = 0;
	virtual void header(std::string const &name, std::string const &value) = 0;

	virtual void redirect(std::string const &url) = 0;
	virtual void expire_time(http_date const &exp) = 0;
	virtual void expire_time(std::string const &exp) = 0;

	virtual void set_cookie(cookie const &c) = 0;
	virtual std::size_t write(char const *buffer, std::size_t size) = 0;

private:
	response(response const &) = delete;
	response& operator = (response const &) = delete;
};

} // namespace

#endif // FASTCGI_DAEMON_RESPONSE_HPP_INCLUDED
