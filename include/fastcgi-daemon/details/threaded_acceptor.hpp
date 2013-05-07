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

#ifndef FASTCGI_DAEMON_DETAILS_THREADED_ACCEPTOR_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_THREADED_ACCEPTOR_HPP_INCLUDED

#include <list>
#include <exception>

#include <boost/ref.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/logger.hpp"
#include "fastcgi-daemon/typedefs.hpp"
#include "fastcgi-daemon/details/fatal_error.hpp"

namespace fcgid { namespace details {

template <typename UrlMatcher>
class threaded_acceptor : private virtual boost::thread_group {

public:
	typedef UrlMatcher matcher_type;
	typedef threaded_acceptor<UrlMatcher> type;
	typedef typename matcher_type::context_type context_type;

	typedef typename context_type::io_type io_type;
	typedef typename io_type::descriptor_type descriptor_type;

	threaded_acceptor();
	virtual ~threaded_acceptor();

	void stop();
	void start(thread_count_type nthreads);
	
	void listen(descriptor_type const &ep);
	void attach_logger(boost::shared_ptr<logger> const &log);

private:
	threaded_acceptor(threaded_acceptor const &);
	threaded_acceptor& operator = (threaded_acceptor const &);
	
	bool stopped() const;
	void stopped(bool value);
	void run_accept_loop(descriptor_type const &desc);

	typedef std::list<descriptor_type> descriptor_list_type;

private:
	bool stopped_;
	matcher_type matcher_;
	boost::shared_ptr<logger> log_;
	descriptor_list_type descriptors_;
	mutable boost::mutex stopped_mutex_;
};

template <typename UrlMatcher> inline
threaded_acceptor<UrlMatcher>::threaded_acceptor() :
	boost::thread_group(), stopped_(false)
{
}

template <typename UrlMatcher> inline
threaded_acceptor<UrlMatcher>::~threaded_acceptor() {
	try {
		stop();
	}
	catch (std::exception const &e) {
	}
}

template <typename UrlMatcher> inline void
threaded_acceptor<UrlMatcher>::stop() {
	if (!stopped()) {
		stopped(true);
		join_all();
	}
}

template <typename UrlMatcher> inline void
threaded_acceptor<UrlMatcher>::start(thread_count_type nthreads) {
	
	std::size_t nth = nthreads.get();
	try {
		for (typename descriptor_list_type::iterator i = descriptors_.begin(), end = descriptors_.end(); i != end; ++i) {
			boost::function<void()> f = boost::bind(&type::run_accept_loop, this, boost::cref(*i));
			for (std::size_t thread_count = 0; thread_count < nth; ++thread_count) {
				create_thread(f);
			}
		}
	}
	catch (std::exception const &e) {
		stop();
		log_->error("exception caught while starting server: %s", e.what());
		throw;
	}
}

template <typename UrlMatcher> inline void
threaded_acceptor<UrlMatcher>::listen(typename threaded_acceptor<UrlMatcher>::descriptor_type const &desc) {
	for (typename descriptor_list_type::iterator i = descriptors_.begin(), end = descriptors_.end(); i != end; ++i) {
		if (desc == *i) {
			throw runtime_error("can not listen endpoint twice");
		}
	}
	descriptors_.push_back(desc);
}

template <typename UrlMatcher> inline void
threaded_acceptor<UrlMatcher>::attach_logger(boost::shared_ptr<logger> const &log) {
	log_ = log;
}

template <typename UrlMatcher> inline bool
threaded_acceptor<UrlMatcher>::stopped() const {
	boost::mutex::scoped_lock sl(stopped_mutex_);
	return stopped_;
}

template <typename UrlMatcher> inline void
threaded_acceptor<UrlMatcher>::stopped(bool value) {
	boost::mutex::scoped_lock sl(stopped_mutex_);
	stopped_ = value;
}

template <typename UrlMatcher> inline void
threaded_acceptor<UrlMatcher>::run_accept_loop(typename threaded_acceptor<UrlMatcher>::descriptor_type const &desc) {
	while (!stopped()) {
		try {
			boost::shared_ptr<context_type> ctx(new context_type(desc));
			ctx->accept();
			matcher_.handle(ctx, *log_);
			ctx->finish();
		}
		catch (fatal_error const &) {
			stop();
		}
		catch (std::exception const &e) {
		}
	}
}

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_THREADED_ACCEPTOR_HPP_INCLUDED
