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

#ifndef FASTCGI_STATISTICS_CONFIG_HPP_INCLUDED
#define FASTCGI_STATISTICS_CONFIG_HPP_INCLUDED

#include <cstddef>
#include <boost/ref.hpp>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/forward.hpp"
#include "fastcgi-daemon/typedefs.hpp"

namespace fcgid { 

class FASTCGI_DAEMON_API statistics {

public:
	typedef details::statistics_impl impl_type;
	explicit statistics(impl_type const &impl);

	statistics(statistics const &other);
	statistics& operator = (statistics const &other);
	void swap(statistics &other) throw ();
	
	std::size_t requests_handled() const;
	std::size_t exceptions_occured() const;
	std::size_t client_errors_occured() const;
	std::size_t server_errors_occured() const;

	std::size_t requests_accepted() const;
	std::size_t requests_in_queue(queue_name_type queue) const;

private:
	boost::reference_wrapper<impl_type const> impl_;
};

inline FASTCGI_DAEMON_API void
swap(statistics &target, statistics &other) {
	target.swap(other);
}

} // namespace

#endif // FASTCGI_STATISTICS_CONFIG_HPP_INCLUDED
