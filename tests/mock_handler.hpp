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

#ifndef FASTCGI_DAEMON_TESTS_MOCK_HANDLER_HPP_INCLUDED
#define FASTCGI_DAEMON_TESTS_MOCK_HANDLER_HPP_INCLUDED

#include <string>
#include <boost/shared_ptr.hpp>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/forward.hpp"

namespace fcgid { namespace tests {

class mock_request {

public:
	mock_request(std::string const &pi);
	std::string const& path_info() const;
	typedef std::string string_type;

private:
	mock_request(mock_request const &);
	mock_request& operator = (mock_request const &);

private:
	std::string path_info_;
};

class mock_response {

public:
	mock_response();
	std::string const& content() const;
	void content(std::string const &cont);
	
private:
	mock_response(mock_response const &);
	mock_response& operator = (mock_response const &);

private:
	std::string content_;
};

class mock_context {

public:
	mock_context(std::string const &pi);
	mock_response& response();
	mock_request const& request() const;

	typedef mock_request request_type;
	typedef mock_response response_type;

private:
	mock_request request_;
	mock_response response_;
};

class mock_handler {

public:
	mock_handler();
	mock_handler(std::string const &name);
	void handle(boost::shared_ptr<mock_context> const &ctx, logger &log) const;
	
	typedef mock_context context_type;
	
	class bool_convertible;
	operator bool_convertible const* () const;

private:
	std::string name_;
};

}} // namespaces

#endif // FASTCGI_DAEMON_TESTS_MOCK_HANDLER_HPP_INCLUDED
