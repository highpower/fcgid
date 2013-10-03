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
#include <exception>

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/typedefs.hpp"
#include "fastcgi-daemon/details/sync_queue.hpp"
#include "fastcgi-daemon/details/runtime_error.hpp"
#include "fastcgi-daemon/details/default_invoker.hpp"

namespace fcgid { namespace details {

template <typename Handler>
class threaded_invoker_task {

public:
	typedef Handler handler_type;
	typedef typename handler_type::context_type context_type;

	threaded_invoker_task();
	threaded_invoker_task(handler_type const &handler, boost::shared_ptr<context_type> const &ctx);

	handler_type const& handler() const;
	boost::shared_ptr<context_type> const& context() const;
	
private:	
	handler_type const *handler_;
	boost::shared_ptr<context_type> context_;
};

template <typename Handler>
class threaded_invoker_descriptor;

template <typename Handler>
class threaded_invoker : public default_invoker<Handler>, private boost::thread_group {

public:
	threaded_invoker();
	virtual ~threaded_invoker();

	typedef Handler handler_type;
	typedef threaded_invoker<Handler> type;
	typedef default_invoker<Handler> base_type;
	typedef typename handler_type::context_type context_type;
	typedef threaded_invoker_descriptor<Handler> descriptor_type;

	typedef threaded_invoker_task<Handler> task_type;
	typedef static_bounded_sync_queue<task_type, 10000> task_queue_type;

	void stop();
	void create_queue(queue_name_type name, thread_count_type nthreads);
	descriptor_type descriptor(queue_name_type name, handler_type const &handler);

private:
	threaded_invoker(threaded_invoker const &);
	threaded_invoker& operator = (threaded_invoker const &);
	void poll_queue(boost::shared_ptr<task_queue_type> queue);
	
	typedef boost::shared_ptr<task_queue_type> task_queue_ptr_type;
	typedef std::map<std::string, task_queue_ptr_type> queue_map_type;
	
private:
	queue_map_type queues_;
};

template <typename Handler>
class threaded_invoker_descriptor {
	
public:
	typedef Handler handler_type;
	typedef typename handler_type::context_type context_type;

	threaded_invoker_descriptor();
	threaded_invoker_descriptor(handler_type const &handler, boost::shared_ptr<typename threaded_invoker<Handler>::task_queue_type> const &queue);
	void invoke(boost::shared_ptr<context_type> const &ctx);

private:
	handler_type handler_;	
	boost::shared_ptr<typename threaded_invoker<Handler>::task_queue_type> queue_;
};

template <typename Handler> inline 
threaded_invoker_task<Handler>::threaded_invoker_task() :
	handler_(), context_()
{
}

template <typename Handler> inline 
threaded_invoker_task<Handler>::threaded_invoker_task(typename threaded_invoker_task<Handler>::handler_type const &handler, boost::shared_ptr<typename threaded_invoker_task<Handler>::context_type> const &ctx) :
	handler_(&handler), context_(ctx)
{
}

template <typename Handler> inline typename threaded_invoker_task<Handler>::handler_type const&
threaded_invoker_task<Handler>::handler() const {
	return *handler_;
}

template <typename Handler> inline boost::shared_ptr<typename threaded_invoker_task<Handler>::context_type> const&
threaded_invoker_task<Handler>::context() const {
	return context_;
}

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
	for (typename queue_map_type::iterator i = queues_.begin(), end = queues_.end(); i != end; ++i) {
		i->second->stop();
	}
	join_all();
}

template <typename Handler> inline void
threaded_invoker<Handler>::create_queue(queue_name_type name, thread_count_type nthreads) {
	typename queue_map_type::iterator i = queues_.find(name.get());
	if (queues_.end() == i) {
		boost::shared_ptr<task_queue_type> queue(new task_queue_type(name.get()));
		try {
			std::size_t nth = nthreads.get();
			boost::function<void()> f(boost::bind(&threaded_invoker<Handler>::poll_queue, this, queue));
			for (std::size_t i = 0; i < nth; ++i) {
				create_thread(f);
			}
			queues_.insert(std::make_pair(std::string(name.get()), queue));
		}
		catch (std::exception const &e) {
			queue->stop();
		}
	}
	throw runtime_error("second queue named %s", name.get());
}

template <typename Handler> inline typename threaded_invoker<Handler>::descriptor_type
threaded_invoker<Handler>::descriptor(queue_name_type name, typename threaded_invoker<Handler>::handler_type const &handler) {
	typename queue_map_type::const_iterator i = queues_.find(name.get());
	if (queues_.end() == i) {
		throw runtime_error("no queue named %s", name.get());
	}
	return descriptor_type(handler, i->second);
}

template <typename Handler> inline void
threaded_invoker<Handler>::poll_queue(boost::shared_ptr<typename threaded_invoker<Handler>::task_queue_type> queue) {
	while (true) {
		try {
			std::pair<task_type, bool> result = queue->pop();
			if (!result.second) {
				threaded_invoker_task<Handler> &task = result.first;
				base_type::handle(task.handler(), task.context());
			}
			else {
				break;
			}
		}
		catch (std::exception const &e) {
			logger &log = base_type::log();
			log.error("exception occured while popping request from queue: %s", e.what());
		}
	}
}

template <typename Handler> inline 
threaded_invoker_descriptor<Handler>::threaded_invoker_descriptor() :
	handler_(), queue_()
{
}

template <typename Handler> inline 
threaded_invoker_descriptor<Handler>::threaded_invoker_descriptor(typename threaded_invoker_descriptor<Handler>::handler_type const &handler, boost::shared_ptr<typename threaded_invoker<Handler>::task_queue_type> const &queue) :
	handler_(handler), queue_(queue)
{
}

template <typename Handler> inline void
threaded_invoker_descriptor<Handler>::invoke(boost::shared_ptr<typename threaded_invoker_descriptor<Handler>::context_type> const &ctx) {
	threaded_invoker_task<Handler> task(handler_, ctx);
	queue_->push(task);
}

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_THREADED_INVOKER_HPP_INCLUDED
