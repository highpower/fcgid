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

#ifndef FASTCGI_DAEMON_DETAILS_SYNC_QUEUE_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_SYNC_QUEUE_HPP_INCLUDED

#include <queue>
#include <utility>
#include <functional>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

#include "fastcgi-daemon/error.hpp"
#include "fastcgi-daemon/config.hpp"

namespace fcgid { namespace details {

class bounds_reached : public error {
public:
	virtual char const* what() const throw ();
};

template <typename BoundsChecker, typename Item>
class sync_queue_base : private BoundsChecker {

public:
	sync_queue_base();
	virtual ~sync_queue_base();
	
	void stop();
	bool stopped() const;
	
	std::pair<Item, bool> pop();
	void push(Item const &item);
	
private:
	sync_queue_base(sync_queue_base const &);
	sync_queue_base& operator = (sync_queue_base const &);
	
	bool ready() const;
	using BoundsChecker::reached;

private:
	bool stopped_;
	std::queue<Item> items_;
	boost::condition condition_;
	boost::mutex mutable mutex_;
	boost::function<bool()> ready_;
};

struct null_bounds_checker {
	bool reached(std::size_t size) const;
};

template <std::size_t N>
struct static_bounds_checker {
	bool reached(std::size_t size) const;
};

class dynamic_bounds_checker {

public:
	dynamic_bounds_checker();
	void bounds(std::size_t size);
	bool reached(std::size_t size) const;

private:
	std::size_t size_;
};

template <typename Item>
struct unbounded_sync_queue : public sync_queue_base<null_bounds_checker, Item> {
};

template <typename Item, std::size_t N>
struct static_bounded_sync_queue : public sync_queue_base<static_bounds_checker<N>, Item> {
};

template <typename Item>
struct dynamic_bounded_sync_queue : public sync_queue_base<dynamic_bounds_checker, Item> {
};

template <typename BoundsChecker, typename Item> inline
sync_queue_base<BoundsChecker, Item>::sync_queue_base() :
	stopped_(false), ready_()
{
	ready_ = boost::bind(&sync_queue_base<BoundsChecker, Item>::ready, this);
}

template <typename BoundsChecker, typename Item> inline
sync_queue_base<BoundsChecker, Item>::~sync_queue_base() {
}

template <typename BoundsChecker, typename Item> inline void
sync_queue_base<BoundsChecker, Item>::stop() {
	boost::mutex::scoped_lock lock(mutex_);
	stopped_ = true;
	condition_.notify_all();
}

template <typename BoundsChecker, typename Item> inline bool
sync_queue_base<BoundsChecker, Item>::stopped() const {
	boost::mutex::scoped_lock lock(mutex_);
	return stopped_;
}

template <typename BoundsChecker, typename Item> inline std::pair<Item, bool>
sync_queue_base<BoundsChecker, Item>::pop() {
	
	boost::mutex::scoped_lock lock(mutex_);
	std::pair<Item, bool> result(Item(), false);
	condition_.wait(lock, ready_);
	if (!stopped_) {
		result = std::make_pair(items_.front(), true);
		items_.pop();
	}
	return result;
}

template <typename BoundsChecker, typename Item> inline void
sync_queue_base<BoundsChecker, Item>::push(Item const &item) {
	boost::mutex::scoped_lock lock(mutex_);
	if (reached(items_.size())) {
		throw bounds_reached();
	}
	items_.push(item);
	condition_.notify_all();
}

template <typename BoundsChecker, typename Item> inline bool
sync_queue_base<BoundsChecker, Item>::ready() const {
	return stopped_ || !items_.empty();
}

template <std::size_t N> inline bool
static_bounds_checker<N>::reached(std::size_t size) const {
	return N == size;
}

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_SYNC_QUEUE_HPP_INCLUDED
