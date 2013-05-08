// fastcgi-daemon is an advanced wrapper around libfcgi library written on c++
// Copyright (C) 2009 Rambler <highpower@rambler.ru>

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef FASTCGI_DAEMON_DETAILS_THREADED_INVOKER_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_THREADED_INVOKER_HPP_INCLUDED

#include <map>
#include <queue>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/details/runtime_error.hpp"
#include "fastcgi-daemon/details/default_invoker.hpp"

namespace fcgid { namespace details {

template <typename Handler>
class threaded_queue;

template <typename Handler>
class threaded_invoker_descriptor;

template <typename Handler>
class threaded_invoker : private default_invoker<Handler>, private boost::thread_group {

public:
	threaded_invoker();
	virtual ~threaded_invoker();

	typedef Handler handler_type;
	typedef threaded_invoker<Handler> type;
	typedef typename handler_type::context_type context_type;
	typedef threaded_invoker_descriptor<Handler> descriptor_type;
	typedef default_invoker<Handler> base_type;

	void stop();
	void create_queue(queue_name_type name, std::size_t nthreads);
	descriptor_type descriptor(queue_name_type name, handler_type const &handler);

private:
	threaded_invoker(threaded_invoker const &);
	threaded_invoker& operator = (threaded_invoker const &);

	typedef threaded_queue<Handler> queue_type;	
	void poll_queue(boost::shared_ptr<queue_type> queue);
	
private:
	typedef boost::shared_ptr<queue_type> queue_ptr_type;
	std::map<std::string, queue_ptr_type> queues_;
};

template <typename Handler>
class thread_queue {

public:
	typedef Handler handler_type;
	typedef typename handler_type::context_type context_type;
	
	thread_queue();

private:
	thread_queue(thread_queue const &);
	thread_queue& operator = (thread_queue const &);
};

template <typename Handler>
class threaded_invoker_descriptor {
	
public:
	typedef Handler handler_type;
	typedef threaded_queue<Handler> queue_type;
	typedef typename handler_type::context_type context_type;

	threaded_invoker_descriptor();
	threaded_invoker_descriptor(handler_type const &handler, boost::shared_ptr<queue_type> const &queue);
	void handle(boost::shared_ptr<context_type> const &ctx, logger &log) const;

private:
	handler_type handler_;
	boost::shared_ptr<queue_type> queue_;
};

template <typename Handler> inline 
threaded_invoker<Handler>::threaded_invoker() :
	default_invoker<Handler>()
{
}

template <typename Handler> inline 
threaded_invoker<Handler>::~threaded_invoker() {
}

template <typename Handler> inline void
threaded_invoker<Handler>::stop() {
	for (typename std::map<std::string, queue_ptr_type>::iterator i = queues_.begin(), end = queues_.end(); i != end; ++i) {
		// i->second->stop();
	}
	join_all();
}

template <typename Handler> inline void
threaded_invoker<Handler>::create_queue(queue_name_type name, std::size_t nthreads) {
	queue_ptr_type queue(new queue_type());
	typename std::map<std::string, queue_ptr_type>::iterator i = queues_.find(name);
	if (queues_.end() == i) {
		queues_.push_back(std::make_pair(std::string(name.get()), queue));
	}
	boost::function<void()> f = boost::bind(&threaded_invoker<Handler>::poll_queue, this, queue);
	for (std::size_t count = 0; count < nthreads; ++count) {
		create_thread(f);
	}
}

template <typename Handler> inline typename threaded_invoker<Handler>::descriptor_type
threaded_invoker<Handler>::descriptor(queue_name_type name, typename threaded_invoker<Handler>::handler_type const &handler) {
	std::string key(name.get());
	typename std::map<std::string, queue_ptr_type>::iterator i = queues_.find(key);
	if (queues_.end() != i->second) {
		return descriptor_type(handler, i->second);
	}
	throw runtime_error("no queue named %s found", name.get());
}

template <typename Handler> inline void
threaded_invoker<Handler>::poll_queue(boost::shared_ptr<typename threaded_invoker<Handler>::queue_type> queue) {
	// while (!queue.stopped()) {
	// }
}

template <typename Handler> inline 
threaded_invoker_descriptor<Handler>::threaded_invoker_descriptor() :
	handler_(), queue_()
{
}

template <typename Handler> inline 
threaded_invoker_descriptor<Handler>::threaded_invoker_descriptor(typename threaded_invoker_descriptor<Handler>::handler_type const &handler, boost::shared_ptr<typename threaded_invoker_descriptor<Handler>::queue_type> const &queue) :
	handler_(handler), queue_(queue)
{
}

template <typename Handler> inline void
threaded_invoker_descriptor<Handler>::handle(boost::shared_ptr<typename threaded_invoker_descriptor<Handler>::context_type> const &ctx, logger &log) const {
	// queue_.push_task(handler_, data);
}

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_THREADED_INVOKER_HPP_INCLUDED
