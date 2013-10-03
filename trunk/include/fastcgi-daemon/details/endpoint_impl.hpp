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

#ifndef FASTCGI_DAEMON_DETAILS_ENDPOINT_IMPL_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_ENDPOINT_IMPL_HPP_INCLUDED

#include <list>
#include <string>
#include <boost/thread.hpp>

#include <fcgiapp.h>

#include "fastcgi-daemon/config.hpp"

namespace fcgid { namespace details {

class fastcgi;

class endpoint_impl {

public:
	endpoint_impl(char const *addr, unsigned short backlog);
	virtual ~endpoint_impl();
	
	int socket() const;
	bool operator == (endpoint_impl const &other) const;
	
private:
	endpoint_impl(endpoint_impl const &);
	endpoint_impl& operator = (endpoint_impl const &);
	
private:
	int socket_;
	std::string addr_;
};

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_ENDPOINT_IMPL_HPP_INCLUDED
