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
	
	logger& log();
	void add(boost::shared_ptr<logger> const &log);
	
	descriptor_type descriptor(Handler const &handler);
	void handle(handler_type const &handler, boost::shared_ptr<context_type> const &ctx) throw ();
	
	void info(char const *issue, boost::shared_ptr<context_type> const &ctx);
	void error(char const *issue, boost::shared_ptr<context_type> const &ctx, std::exception const &e);

private:
	default_invoker(default_invoker const &);
	default_invoker& operator = (default_invoker const &);
	
private:
	boost::shared_ptr<logger> log_;
};

template <typename Handler>
class default_invoker_descriptor {

public:
	typedef Handler handler_type;
	typedef typename handler_type::context_type context_type;

	default_invoker_descriptor();
	default_invoker_descriptor(handler_type const &handler, default_invoker<Handler> *invoker);
	
	void invoke(boost::shared_ptr<context_type> const &ctx) const;
	
private:
	handler_type handler_;
	default_invoker<Handler> *invoker_;
};

template <typename Handler> inline
default_invoker<Handler>::default_invoker() :
	log_()
{
}

template <typename Handler> inline 
default_invoker<Handler>::~default_invoker() {
}

template <typename Handler> inline logger&
default_invoker<Handler>::log() {
	return *log_;
}

template <typename Handler> inline void
default_invoker<Handler>::add(boost::shared_ptr<logger> const &log) {
	log_ = log;
}

template <typename Handler> inline typename default_invoker<Handler>::descriptor_type
default_invoker<Handler>::descriptor(typename default_invoker<Handler>::handler_type const &handler) {
	return descriptor_type(handler, this);
}

template <typename Handler> inline void
default_invoker<Handler>::handle(typename default_invoker<Handler>::handler_type const &handler, boost::shared_ptr<typename default_invoker<Handler>::context_type> const &ctx) throw () {
	try {
		info("request started", ctx);
		handler.handle(ctx, log());
		info("request handled", ctx);
	}
	catch (http_error const &e) {
		error("exception occured", ctx, e);
	}
	catch (std::exception const &e) {
		error("http error occured", ctx, e);
	}
}

template <typename Handler> inline void
default_invoker<Handler>::info(char const *issue, boost::shared_ptr<typename default_invoker<Handler>::context_type> const &ctx) {
	typedef typename context_type::request_type request_type;
	request_type const &req = ctx->request();
	typename request_type::string_type const &pi = req.path_info();
	log_->info("%s : %s", issue, pi.c_str());
}

template <typename Handler> inline void
default_invoker<Handler>::error(char const *issue, boost::shared_ptr<typename default_invoker<Handler>::context_type> const &ctx, std::exception const &e) {
	typedef typename context_type::request_type request_type;
	request_type const &req = ctx->request();
	typename request_type::string_type const &pi = req.path_info();
	log_->error("%s occured while handling %s: %s", issue, pi.c_str(), e.what());
}

template <typename Handler> inline
default_invoker_descriptor<Handler>::default_invoker_descriptor() :
	handler_(), invoker_(0)
{
}

template <typename Handler> inline
default_invoker_descriptor<Handler>::default_invoker_descriptor(typename default_invoker_descriptor<Handler>::handler_type const &handler, default_invoker<Handler> *invoker) :
	handler_(handler), invoker_(invoker)
{
}

template <typename Handler> inline void
default_invoker_descriptor<Handler>::invoke(boost::shared_ptr<typename default_invoker_descriptor<Handler>::context_type> const &ctx) const {
	if (!invoker_ || !handler_) {
		throw http_error(http_status::not_found);
	}
	invoker_->handle(handler_, ctx);
}

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_DEFAULT_INVOKER_HPP_INCLUDED
