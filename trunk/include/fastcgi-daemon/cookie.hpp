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

#ifndef FASTCGI_DAEMON_COOKIE_HPP_INCLUDED
#define FASTCGI_DAEMON_COOKIE_HPP_INCLUDED

#include <string>
#include <boost/shared_ptr.hpp>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/forward.hpp"

namespace fcgid {

class FASTCGI_DAEMON_API cookie {

public:
	typedef details::cookie_impl<stdalloc> impl_type;
	
	cookie(std::string const &name, std::string const &value);
	virtual ~cookie();

	cookie(cookie const &other);
	cookie& operator = (cookie const &other);
	void swap(cookie &other) throw ();
	
	std::string const& name() const;
	std::string const& value() const;

	bool secure() const;
	void secure(bool value);
	
	bool http_only() const;
	void http_only(bool value);

	std::string const& path() const;
	void path(std::string const &value);

	std::string const& domain() const;
	void domain(std::string const &value);

	http_date const& expire_time() const;
	void expire_time(http_date const& value);
	void expire_time(std::string const &value);

	std::string str() const;
	boost::shared_ptr<impl_type> const& impl() const;

private:
	boost::shared_ptr<impl_type> impl_;
};

inline FASTCGI_DAEMON_API void
swap(cookie &target, cookie &other) throw () {
	target.swap(other);
}

} // namespace

#endif // FASTCGI_DAEMON_COOKIE_HPP_INCLUDED
