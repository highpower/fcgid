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

#ifndef FASTCGI_DAEMON_DETAILS_STANDARD_HANDLER_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_STANDARD_HANDLER_HPP_INCLUDED

#include <boost/shared_ptr.hpp>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/forward.hpp"
#include "fastcgi-daemon/http_method.hpp"

#include "fastcgi-daemon/details/context.hpp"
#include "fastcgi-daemon/details/fastcgi.hpp"
#include "fastcgi-daemon/details/standard_request.hpp"
#include "fastcgi-daemon/details/standard_response.hpp"

namespace fcgid { namespace details {

class standard_handler {

public:
	standard_handler();
	virtual ~standard_handler();
	
	typedef standard_request<fastcgi> request_type;
	typedef standard_response<fastcgi> response_type;
	typedef context<fastcgi, request_type, response_type> context_type;

	standard_handler(standard_handler const &other);
	standard_handler& operator = (standard_handler const &other);
	void swap(standard_handler &other) throw ();
	
	standard_handler(boost::shared_ptr<request_handler> const &handler, http_method_mask const &m);
	void handle(boost::shared_ptr<context_type> ctx, logger &log) const;

	class bool_convertible;
	operator bool_convertible const* () const;

private:
	http_method_mask mask_;
	boost::shared_ptr<request_handler> handler_;
};

inline void
swap(standard_handler &target, standard_handler &other) {
	target.swap(other);
}

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_STANDARD_HANDLER_HPP_INCLUDED
