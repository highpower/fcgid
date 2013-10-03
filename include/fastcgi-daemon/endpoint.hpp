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

#ifndef FASTCGI_DAEMON_ENDPOINT_HPP_INCLUDED
#define FASTCGI_DAEMON_ENDPOINT_HPP_INCLUDED

#include <memory>
#include <string>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/forward.hpp"
#include "fastcgi-daemon/distinct.hpp"

namespace fcgid {

class FASTCGI_DAEMON_API endpoint {

public:
	endpoint();
	virtual ~endpoint();
	
	endpoint(endpoint const &other);
	endpoint& operator = (endpoint const &other);
	void swap(endpoint &other) throw ();

	bool operator == (endpoint const &other) const;
	bool operator != (endpoint const &other) const;

	static endpoint create_unix_socket(char const *path, unsigned short backlog);
	static endpoint create_netword_socket(char const *addr, unsigned short port, unsigned short backlog);
	
	typedef details::endpoint_impl impl_type;
	std::shared_ptr<impl_type> const& impl() const;

private:
	explicit endpoint(std::shared_ptr<impl_type> const &impl);

private:
	std::shared_ptr<impl_type> impl_;
};

inline FASTCGI_DAEMON_API void
swap(endpoint &target, endpoint &other) {
	target.swap(other);
}

} // namespace

#endif // FASTCGI_DAEMON_ENDPOINT_HPP_INCLUDED
