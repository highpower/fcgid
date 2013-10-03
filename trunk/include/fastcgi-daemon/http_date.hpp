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

#ifndef FASTCGI_DAEMON_HTTP_DATE_HPP_INCLUDED
#define FASTCGI_DAEMON_HTTP_DATE_HPP_INCLUDED

#include <ctime>
#include <iostream>
#include <boost/static_assert.hpp>

#include "fastcgi-daemon/config.hpp"

namespace fcgid {

class FASTCGI_DAEMON_API http_date {

public:
	http_date();
	explicit http_date(std::time_t t);
	void swap(http_date &other) throw ();
	
	bool operator == (http_date const &other) const;
	bool operator != (http_date const &other) const;

	std::size_t rfc1123(char *buffer, std::size_t size) const;
	std::size_t rfc1036(char *buffer, std::size_t size) const;
	std::size_t asctime(char *buffer, std::size_t size) const;
	
	static http_date from_now(char const *value);
	static http_date const bad;

private:
	std::time_t when_;
};

inline FASTCGI_DAEMON_API bool
http_date::operator == (http_date const &other) const {
	return when_ == other.when_;
}

inline FASTCGI_DAEMON_API bool
http_date::operator != (http_date const &other) const {
	return when_ != other.when_;
}

inline FASTCGI_DAEMON_API void
swap(http_date &target, http_date &other) throw () {
	target.swap(other);
}

} // namespace

#endif // FASTCGI_DAEMON_HTTP_DATE_HPP_INCLUDED
