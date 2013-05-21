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

#ifndef FASTCGI_DAEMON_DETAILS_SERVER_IMPL_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_SERVER_IMPL_HPP_INCLUDED

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/endpoint.hpp"
#include "fastcgi-daemon/details/urltree.hpp"
#include "fastcgi-daemon/details/threaded_invoker.hpp"
#include "fastcgi-daemon/details/standard_handler.hpp"
#include "fastcgi-daemon/details/threaded_acceptor.hpp"

namespace fcgid { namespace details {

class server_impl : public threaded_acceptor<urltree<threaded_invoker<standard_handler>::type>::type> {

public:
	server_impl();
	virtual ~server_impl();
	
	typedef threaded_invoker<standard_handler> invoker_type;
	typedef urltree<invoker_type> matcher_type;
	typedef threaded_acceptor<matcher_type> base_type;

	matcher_type& matcher();
	invoker_type& invoker();
	
private:
	server_impl(server_impl const &);
	server_impl& operator = (server_impl const &);
};

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_SERVER_IMPL_HPP_INCLUDED
