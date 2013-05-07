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

#ifndef FASTCGI_DAEMON_DETAILS_COOKIE_IMPL_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_COOKIE_IMPL_HPP_INCLUDED

#include <string>
#include <sstream>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/http_date.hpp"
#include "fastcgi-daemon/details/runtime_error.hpp"
#include "fastcgi-daemon/details/urlencode_impl.hpp"

namespace fcgid { namespace details {

template <typename A>
class cookie_impl {

public:
	typedef cookie_impl<A> type;
	typedef typename A::template rebind<char>::other allocator_type;
	typedef std::basic_string<char, std::char_traits<char>, allocator_type> string_type;
	
	cookie_impl(string_type const &name, string_type const &value);

	string_type const& name() const;
	string_type const& value() const;

	bool secure() const;
	void secure(bool value);
	
	bool http_only() const;
	void http_only(bool value);

	string_type const& path() const;
	void path(string_type const &value);

	string_type const& domain() const;
	void domain(string_type const &value);

	http_date const& expire_time() const;
	void expire_time(http_date const& value);
	void expire_time(string_type const &value);

	string_type str() const;

private:
	cookie_impl(cookie_impl const &);
	cookie_impl& operator = (cookie_impl const &);

private:
	http_date expire_time_;
	bool secure_, http_only_;
	string_type const pattern_;
	string_type name_, value_, path_, domain_;
};

template <typename A> inline 
cookie_impl<A>::cookie_impl(typename cookie_impl<A>::string_type const &name, typename cookie_impl<A>::string_type const &value) :
	expire_time_(), secure_(false), http_only_(false), pattern_(name.get_allocator()), name_(name), value_(value), path_(pattern_.get_allocator()), domain_(pattern_.get_allocator())
{
	if (name_.empty()) {
		throw runtime_error("%s", "can not create cookie without name");
	}
}

template <typename A> inline typename cookie_impl<A>::string_type const&
cookie_impl<A>::name() const {
	return name_;
}

template <typename A> inline typename cookie_impl<A>::string_type const&
cookie_impl<A>::value() const {
	return value_;
}

template <typename A> inline bool
cookie_impl<A>::secure() const {
	return secure_;
}

template <typename A> inline void
cookie_impl<A>::secure(bool value) {
	secure_ = value;
}
	
template <typename A> inline bool
cookie_impl<A>::http_only() const {
	return http_only_;
}

template <typename A> inline void
cookie_impl<A>::http_only(bool value) {
	http_only_ = value;
}

template <typename A> inline typename cookie_impl<A>::string_type const&
cookie_impl<A>::path() const {
	return path_;
}

template <typename A> inline void
cookie_impl<A>::path(typename cookie_impl<A>::string_type const &value) {
	path_ = value;
}

template <typename A> inline typename cookie_impl<A>::string_type const&
cookie_impl<A>::domain() const {
	return domain_;
}

template <typename A> inline void
cookie_impl<A>::domain(typename cookie_impl<A>::string_type const &value) {
	domain_ = value;
}


template <typename A> inline http_date const&
cookie_impl<A>::expire_time() const {
	return expire_time_;
}

template <typename A> inline void
cookie_impl<A>::expire_time(http_date const& value) {
	expire_time_ = value;
}

template <typename A> inline void
cookie_impl<A>::expire_time(typename cookie_impl<A>::string_type const &value) {
	// TODO: implement http_date::from_string
	expire_time_ = http_date::from_now(value.c_str());
}

template <typename A> inline typename cookie_impl<A>::string_type
cookie_impl<A>::str() const {
	
	std::basic_ostringstream<char, std::char_traits<char>, A> stream(pattern_);
	stream << urlencode(name_, pattern_) << '=' << urlencode(value_, pattern_);
	if (!domain_.empty()) {
		stream << "; Domain=" << domain_;
	}
	if (!path_.empty()) {
		stream << "; Path=" << urlencode(path_, pattern_);
	}
	if (http_date::bad != expire_time_) {
		// stream <<  "; Expires=" << expire_time_;
	}
	if (http_only_) {
		stream << "; HttpOnly";
	}
	if (secure_) {
		stream << "; Secure";
	}
	return stream.str();

}

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_COOKIE_IMPL_HPP_INCLUDED
