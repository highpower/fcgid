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

#ifndef FASTCGI_DAEMON_SERVER_HPP_INCLUDED
#define FASTCGI_DAEMON_SERVER_HPP_INCLUDED

#include <boost/shared_ptr.hpp>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/forward.hpp"
#include "fastcgi-daemon/typedefs.hpp"

namespace fcgid {

class FASTCGI_DAEMON_API server {

public:
	server();
	virtual ~server();

	server(server const &other);
	server& operator = (server const &other);
	void swap(server &other) throw ();

	void stop();
	void start(thread_count_type nthreads);

	void listen(endpoint const &ep);
	void create_queue(queue_name_type name, thread_count_type nthreads);
	
	void add(boost::shared_ptr<logger> const &log);
	void add(url_type url, queue_name_type queue, boost::shared_ptr<request_handler> const &handler, http_method_mask const &mask);

	statistics stat() const;

private:
	typedef details::server_impl impl_type;
	boost::shared_ptr<impl_type> impl_;
};

} // namespace

#endif // FASTCGI_DAEMON_SERVER_HPP_INCLUDED
