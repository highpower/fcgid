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

#ifndef FASTCGI_DAEMON_DETAILS_FASTCGI_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_FASTCGI_HPP_INCLUDED

#include <fcgiapp.h>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/forward.hpp"
#include "fastcgi-daemon/details/range.hpp"

namespace fcgid { namespace details {

class fastcgi {

public:
	fastcgi(endpoint const &desc);
	virtual ~fastcgi();

	typedef endpoint descriptor_type;

	void accept();
	void finish();

	char const* const* environ() const;
	std::size_t read(char* buffer, std::size_t size);
	
	void status(http_status const &st);
	std::size_t write(char const *buffer, std::size_t size);
	std::size_t write_header(range<char const*> const &name, range<char const*> const &value);

private:
	fastcgi(fastcgi const &);
	fastcgi& operator = (fastcgi const &);

	bool is_accepted() const;
	static void init_fastcgi_engine();

private:
	FCGX_Request req_;
};

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_FASTCGI_HPP_INCLUDED
