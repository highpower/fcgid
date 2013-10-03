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

#ifndef FASTCGI_DAEMON_DETAILS_URLTREE_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_URLTREE_HPP_INCLUDED

#include <map>
#include <list>
#include <memory>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/forward.hpp"
#include "fastcgi-daemon/typedefs.hpp"
#include "fastcgi-daemon/http_error.hpp"
#include "fastcgi-daemon/http_status.hpp"

#include "fastcgi-daemon/details/range.hpp"
#include "fastcgi-daemon/details/tokenizer.hpp"

namespace fcgid { namespace details {

template <typename Invoker>
class urltree_node;

template <typename Invoker>
class urltree {

public:
	typedef Invoker invoker_type;
	typedef urltree<Invoker> type;
	typedef typename invoker_type::context_type context_type;
	typedef typename invoker_type::handler_type handler_type;
	typedef typename invoker_type::descriptor_type descriptor_type;

	urltree();
	virtual ~urltree();
	
	invoker_type& invoker();
	void add(url_type url, descriptor_type const &desc);
	void add(std::shared_ptr<logger> const &log);
	void handle(std::shared_ptr<context_type> const &ctx);

private:
	urltree(urltree const &) = delete;
	urltree& operator = (urltree const &) = delete;
	typedef urltree_node<Invoker> node_type;
	
private:
	invoker_type invoker_;
	std::list<std::string> urls_;
	std::shared_ptr<node_type> root_;
};

struct urltree_node_base {
	static range<char const*> const wildcard_token;
};

template <typename Invoker>
class urltree_node : private urltree_node_base {

public:
	typedef urltree_node<Invoker> type;
	typedef typename Invoker::context_type context_type;
	typedef typename Invoker::descriptor_type descriptor_type;

	urltree_node();
	virtual ~urltree_node();
	
	void descriptor(descriptor_type const &desc);
	std::shared_ptr<type> get(range<char const*> const &path);
	
	void invoke(std::shared_ptr<context_type> const &ctx);
	type* find(range<char const*> const &path);

private:
	typedef range<char const*> range_type;
	typedef std::shared_ptr<type> ptr_type;
	typedef std::map<range_type, ptr_type> children_map_type;

	using urltree_node_base::wildcard_token;

private:
	ptr_type wildcard_node_;
	descriptor_type descriptor_;
	children_map_type children_;
};

template <typename Invoker> inline
urltree<Invoker>::urltree() :
	root_(new node_type())
{
}

template <typename Invoker> inline
urltree<Invoker>::~urltree() {
}

template <typename Invoker> inline typename urltree<Invoker>::invoker_type&
urltree<Invoker>::invoker() {
	return invoker_;
}

template <typename Invoker> inline void
urltree<Invoker>::add(url_type url, typename urltree<Invoker>::descriptor_type const &desc) {
	
	typedef range<char const*> range_type;
	typedef split_if_equal<char> predicate_type;
	typedef tokenizer<range_type, predicate_type> tokenizer_type;
	
	urls_.push_back(std::string(url.get()));
	std::string const &urlstr = urls_.back();
	range_type range(urlstr.c_str(), urlstr.c_str() + urlstr.size());
	
	std::shared_ptr<node_type> node = root_;
	tokenizer_type tok(range, predicate_type('/'));
	for (tokenizer_type::const_iterator i = tok.begin(), end = tok.end(); i != end; ++i) {
		node = node->get(*i);
	}
	node->descriptor(desc);
}

template <typename Invoker> inline void
urltree<Invoker>::add(std::shared_ptr<logger> const &log) {
	invoker_.add(log);
}

template <typename Invoker> inline void
urltree<Invoker>::handle(std::shared_ptr<typename urltree<Invoker>::context_type> const &ctx) {
	
	typedef range<char const*> range_type;
	typedef split_if_equal<char> predicate_type;
	typedef tokenizer<range_type, predicate_type> tokenizer_type;
	typedef typename context_type::request_type request_type;
	
	request_type const &req = ctx->request();
	typename request_type::string_type const &pi = req.path_info();

	node_type *node = root_.get();
	tokenizer_type tok(make_range(pi.c_str(), pi.c_str() + pi.size()), split_if_equal<char>('/'));
	for (tokenizer_type::const_iterator i = tok.begin(), end = tok.end(); i != end; ++i) {
		node = node->find(*i);
	}
	node->invoke(ctx);
}

template <typename Invoker> inline 
urltree_node<Invoker>::urltree_node() :
	descriptor_(), children_()
{
}

template <typename Invoker> inline 
urltree_node<Invoker>::~urltree_node() {
}

template <typename Invoker> inline void
urltree_node<Invoker>::descriptor(typename urltree_node<Invoker>::descriptor_type const &desc) {
	descriptor_ = desc;
}

template <typename Invoker> inline std::shared_ptr<typename urltree_node<Invoker>::type>
urltree_node<Invoker>::get(range<char const*> const &path) {
	if (wildcard_token == path) {
		if (!wildcard_node_) {
			wildcard_node_.reset(new type());
		}
		return wildcard_node_;
	}
	typename children_map_type::iterator i = children_.find(path);
	if (children_.end() == i) {
		ptr_type ptr(new type());
		children_.insert(std::make_pair(path, ptr));
		return ptr;
	}
	else {
		return i->second;
	}
}

template <typename Invoker> inline void
urltree_node<Invoker>::invoke(std::shared_ptr<typename urltree_node<Invoker>::context_type> const &ctx) {
	descriptor_.invoke(ctx);
}

template <typename Invoker> inline typename urltree_node<Invoker>::type*
urltree_node<Invoker>::find(range<char const*> const &path) {
	typename children_map_type::const_iterator i = children_.find(path);
	if (children_.end() != i) {
		return i->second.get();
	}
	else if (wildcard_node_) {
		return wildcard_node_.get();
	}
	throw http_error(http_status::not_found);
}

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_URLTREE_HPP_INCLUDED
