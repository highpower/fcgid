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

#ifndef FASTCGI_DAEMON_DETAILS_GENERIC_RESPONSE_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_GENERIC_RESPONSE_HPP_INCLUDED

#include <set>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <type_traits>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/http_date.hpp"
#include "fastcgi-daemon/http_status.hpp"
#include "fastcgi-daemon/details/range.hpp"
#include "fastcgi-daemon/details/strings.hpp"
#include "fastcgi-daemon/details/functors.hpp"
#include "fastcgi-daemon/details/cookie_impl.hpp"

namespace fcgid { namespace details {

struct generic_response_base {
	static range<char const*> const status_token;
	static range<char const*> const expire_time_token;
	static range<char const*> const location_token;
	static range<char const*> const set_cookie_token;
};

template <typename IO, typename A>
class generic_response : private generic_response_base {

public:
	typedef cookie_impl<A> cookie_type;
	typedef generic_response<IO, A> type;

	typedef typename A::template rebind<char>::other allocator_type;
	typedef std::basic_string<char, std::char_traits<char>, allocator_type> string_type;
	
	generic_response(IO &io, A const &a = A());
	virtual ~generic_response();

	void status(http_status const &st);
	void header(string_type const &name, string_type const &value);

	void redirect(string_type const &url);
	void expire_time(http_date const &exp);
	void expire_time(string_type const &exp);
	
	void set_cookie(std::shared_ptr<cookie_type> const &cookie);
	std::size_t write(char const *buffer, std::size_t size);

	void clear();
	void send_headers();

private:
	generic_response(generic_response const &);
	generic_response& operator = (generic_response const &);

	using generic_response_base::status_token;
	using generic_response_base::expire_time_token;
	using generic_response_base::location_token;
	using generic_response_base::set_cookie_token;

	typedef std::pair<string_type const, string_type> header_type;
	typedef std::shared_ptr<typename cookie_impl<A>::type> cookie_ptr_type;
	typedef typename A::template rebind<header_type>::other header_allocator_type;
	typedef std::map<string_type, string_type, std::less<string_type>, header_allocator_type> header_map_type;

	struct cookie_ptr_less : public std::binary_function<cookie_ptr_type const &, cookie_ptr_type const &, bool> {
		bool operator () (cookie_ptr_type const &cookie, cookie_ptr_type const &other) const;
	};

	typedef typename A::template rebind<cookie_ptr_type>::other cookie_ptr_allocator_type;
	typedef std::set<cookie_ptr_type, cookie_ptr_less, cookie_ptr_allocator_type> cookie_set_type;
	
private:
	IO &io_;
	bool headers_sent_;
	http_status status_;
	http_date expire_time_;
	cookie_set_type cookies_;
	header_map_type headers_;
	string_type const pattern_;
};

struct header_constants {
	static range<char const*> const p3p_header_token;
};

template <typename Iter> http_status parse_status(Iter begin, Iter end);
template <typename String> String sanitize_header_name(String const &header);

template <typename IO, typename A> inline
generic_response<IO, A>::generic_response(IO &io, A const &a) :
	io_(io), headers_sent_(false), status_(http_status::bad), expire_time_(), 
	cookies_(cookie_ptr_less(), cookie_ptr_allocator_type()), 
	headers_(std::less<string_type>(), header_allocator_type(a)),
	pattern_(allocator_type(a))
	
{
}

template <typename IO, typename A> inline 
generic_response<IO, A>::~generic_response() {
}

template <typename IO, typename A> inline void
generic_response<IO, A>::status(http_status const &st) {
	status_ = st;
	headers_sent_ = false;
}

template <typename IO, typename A> inline void
generic_response<IO, A>::header(typename generic_response<IO, A>::string_type const &name, typename generic_response<IO, A>::string_type const &value) {

	range<typename string_type::const_iterator> name_range = make_range(name.begin(), name.end());
	if (is_ci_equal(status_token, name_range)) {
		status(parse_status(value.begin(), value.end()));
	}
	else if (is_ci_equal(expire_time_token, name_range)) {
		expire_time(value);
	}
	else {
		headers_sent_ = false;	
		headers_[name] = value;
	}
}

template <typename IO, typename A> inline void
generic_response<IO, A>::redirect(typename generic_response<IO, A>::string_type const &url) {
	status(http_status::moved_permanently);
	string_type name(location_token.begin(), location_token.end(), pattern_.get_allocator());
	header(name, url);
}

template <typename IO, typename A> inline void
generic_response<IO, A>::expire_time(http_date const &exp) {
}

template <typename IO, typename A> inline void
generic_response<IO, A>::expire_time(typename generic_response<IO, A>::string_type const &exp) {
}

template <typename IO, typename A> inline void
generic_response<IO, A>::set_cookie(std::shared_ptr<typename generic_response<IO, A>::cookie_type> const &cookie) {
	cookies_.insert(cookie);
	headers_sent_ = false;
}

template <typename IO, typename A> inline std::size_t
generic_response<IO, A>::write(char const *buffer, std::size_t size) {
	if (http_status::no_content == status_) {
		throw runtime_error("writing content while http status doesn't not allow it");
	}
	if (!headers_sent_) {
		send_headers();
	}
	return io_.write(buffer, size);
}

template <typename IO, typename A> inline void
generic_response<IO, A>::send_headers() {

	if (http_status::bad == status_) {
		throw runtime_error("http status has not been set explicitly");
	}
	io_.status(status_);
	io_.write_header(status_token, make_range(status_.message()));
		/*
		if (HttpDate::BAD != expireTime_) {
			io_.writeHeader(HttpConstants::EXPIRES, expireTime_.str());
		}
		*/
	for (typename cookie_set_type::const_iterator i = cookies_.begin(), end = cookies_.end(); i != end; ++i) {
		string_type value = (*i)->str();
		range<char const*> vr(value.c_str(), value.c_str() + value.size());
		io_.write_header(set_cookie_token, vr);
	}
	for (typename header_map_type::const_iterator i = headers_.begin(), end = headers_.end(); i != end; ++i) {
		string_type name = sanitize_header_name(i->first);
		range<char const*> nr(name.c_str(), name.c_str() + name.size());
			
		string_type const &value = i->second;
		range<char const*> vr(value.c_str(), value.c_str() + value.size());

		io_.write_header(nr, vr);
	}
	headers_sent_ = true;
}

template <typename IO, typename A> inline bool
generic_response<IO, A>::cookie_ptr_less::operator () (typename generic_response<IO, A>::cookie_ptr_type const &cookie, typename generic_response<IO, A>::cookie_ptr_type const &other) const {
	return cookie->name() < other->name();
}

template <typename Iter> inline http_status
parse_status(Iter begin, Iter end) {

	typedef typename std::iterator_traits<Iter>::value_type char_type;
	try {
		long status;
		std::stringstream stream;
		stream.exceptions(std::ios::badbit | std::ios::failbit);
		stream << make_range(begin, next_not_matched(begin, end, is_numeric<char_type>()));
		stream >> status;
		return http_status(status);	
	}
	catch (std::ios::failure const &e) {
		throw runtime_error("can not parse status");
	}
}

template <typename String> inline String
sanitize_header_name(String const &str) {

	typedef range<typename String::const_iterator> range_type;
	range_type name(str.begin(), str.end());
	
	if (header_constants::p3p_header_token == name) {
		return str;
	}
	
	bool need_upper = true;
	range_type trimmed = trim(name);
	
	String result(str.get_allocator());
	result.reserve(trimmed.size());
	
	typedef typename std::remove_cv<typename String::value_type>::type char_type;
	for (typename range_type::const_iterator i = trimmed.begin(), end = trimmed.end(); i != end; ++i) {
		if ((*i) == static_cast<char_type>('-')) {
			need_upper = true;
			result.push_back(*i);
		}
		else if (need_upper && char_traits<char_type>::is_alpha_numeric(*i)) {
			need_upper = false;
			result.push_back(char_traits<char_type>::to_upper(*i));
		}
		else {
			result.push_back(char_traits<char_type>::to_lower(*i));
		}
	}
	return result;
}

/*
template <typename IO> NEXTWEB_INLINE void
generic_response<IO>::clear() {
	cookies_.clear();
	headers_.clear();
	expireTime_ = HttpDate::BAD;
	httpStatus_ = HttpStatus::BAD;
}
*/

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_GENERIC_RESPONSE_HPP_INCLUDED
