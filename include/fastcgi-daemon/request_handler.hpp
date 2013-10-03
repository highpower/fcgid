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

#ifndef FASTCGI_DAEMON_REQUEST_HANDLER_HPP_INCLUDED
#define FASTCGI_DAEMON_REQUEST_HANDLER_HPP_INCLUDED

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/forward.hpp"

namespace fcgid {

class FASTCGI_DAEMON_API request_handler {

public:
	request_handler();
	virtual ~request_handler();
	virtual void handle_request(request const &req, response &resp, logger &log) = 0;

private:
	request_handler(request_handler const &) = delete;
	request_handler& operator = (request_handler const &) = delete;
};

} // namespace

#endif // FASTCGI_DAEMON_REQUEST_HANDLER_HPP_INCLUDED
