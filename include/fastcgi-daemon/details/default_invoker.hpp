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


#ifndef FASTCGI_DAEMON_DETAILS_DEFAULT_INVOKER_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_DEFAULT_INVOKER_HPP_INCLUDED

#include <cassert>
#include <exception>
#include <boost/shared_ptr.hpp>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/logger.hpp"
#include "fastcgi-daemon/typedefs.hpp"
#include "fastcgi-daemon/http_error.hpp"
#include "fastcgi-daemon/details/runtime_error.hpp"

namespace fcgid { namespace details {

template <typename Handler>
class default_invoker_descriptor;

template <typename Handler>
class default_invoker {

public:
	default_invoker();
	virtual ~default_invoker();
	
	typedef Handler handler_type;
	typedef default_invoker<Handler> type;
	typedef typename handler_type::context_type context_type;
	typedef default_invoker_descriptor<Handler> descriptor_type;
	
	void attach(boost::shared_ptr<logger> const &log);
	void handle(handler_type const &handler, boost::shared_ptr<context_type> const &ctx);
	descriptor_type descriptor(queue_name_type queue, Handler const &handler);

private:
	default_invoker(default_invoker const &);
	default_invoker& operator = (default_invoker const &);

private:
	boost::shared_ptr<logger> logger_;
};

template <typename Handler>
class default_invoker_descriptor {

public:
	typedef Handler handler_type;
	typedef typename handler_type::context_type context_type;

	default_invoker_descriptor(handler_type const &handler, default_invoker<Handler> *invoker);
	void invoke(boost::shared_ptr<context_type> const &ctx) throw ();

private:
	handler_type handler_;
	default_invoker<Handler> *invoker_;
};

template <typename Handler> inline
default_invoker<Handler>::default_invoker()
{
}

template <typename Handler> inline 
default_invoker<Handler>::~default_invoker() {
}

template <typename Handler> inline void
default_invoker<Handler>::attach(boost::shared_ptr<logger> const &log) {
	assert(log);
	logger_ = log;
}

template <typename Handler> inline void
default_invoker<Handler>::handle(typename default_invoker<Handler>::handler_type const &handler, boost::shared_ptr<typename default_invoker<Handler>::context_type> const &ctx) {
	try {
		handler.handle(ctx, *logger_);
	}
	catch (http_error const &e) {
		// logger_.info()
	}
	catch (runtime_error const &e) {
		// logger_.error()
	}
	catch (std::exception const &e) {
		// logger_.error()
	}
}

template <typename Handler> inline typename default_invoker<Handler>::descriptor_type
default_invoker<Handler>::descriptor(queue_name_type name, typename default_invoker<Handler>::handler_type const &handler) {
	(void) name;
	return descriptor_type(handler, this);
}

template <typename Handler> inline
default_invoker_descriptor<Handler>::default_invoker_descriptor(typename default_invoker_descriptor<Handler>::handler_type const &handler, default_invoker<Handler> *invoker) :
	handler_(handler), invoker_(invoker)
{
}

template <typename Handler> inline void
default_invoker_descriptor<Handler>::invoke(boost::shared_ptr<typename default_invoker_descriptor<Handler>::context_type> const &ctx) throw () {
	invoker_->handle(handler_, ctx);
}

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_DEFAULT_INVOKER_HPP_INCLUDED