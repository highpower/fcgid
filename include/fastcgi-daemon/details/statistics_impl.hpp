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

#ifndef FASTCGI_DAEMON_DETAILS_STATISTICS_IMPL_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_STATISTICS_IMPL_HPP_INCLUDED

#include <cstddef>

#include "fastcgi-daemon/config.hpp"

namespace fcgid { namespace details {

class statistics_impl {

public:
	statistics_impl();
	virtual ~statistics_impl();
	
	std::size_t requests_handled() const;
	std::size_t exceptions_occured() const;
	std::size_t client_errors_occured() const;
	std::size_t server_errors_occured() const;

	std::size_t requests_accepted() const;
	std::size_t requests_in_queue(char const *name) const;

private:
	statistics_impl(statistics_impl const &);
	statistics_impl& operator = (statistics_impl const &);

private:
	
};

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_STATISTICS_IMPL_HPP_INCLUDED
