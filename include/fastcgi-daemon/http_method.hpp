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

#ifndef FASTCGI_DAEMON_HTTP_METHOD_HPP_INCLUDED
#define FASTCGI_DAEMON_HTTP_METHOD_HPP_INCLUDED

#include <iostream>

#include "fastcgi-daemon/config.hpp"

namespace fcgid {

class http_method;

class FASTCGI_DAEMON_API http_method_mask {

public:
	http_method_mask();
	http_method_mask(http_method const &mth);
	void swap(http_method_mask &other) throw ();

	long mask() const;
	void extend(http_method_mask const &m);
	bool accepts(http_method const &m) const;

	static http_method_mask const all;
	static http_method_mask const none;
	static http_method_mask const get_only;
	static http_method_mask const post_only;

private:
	long mask_;
};

class FASTCGI_DAEMON_API http_method {

public:
	void swap(http_method &other) throw ();
	bool operator == (http_method const &other) const;
	bool operator != (http_method const &other) const;

	long code() const;
	char const* name() const;
	
	static http_method const get;
	static http_method const head;
	static http_method const post;
	static http_method const unknown;
	static http_method parse(char const *str);

private:
	explicit http_method(long code);
	
private:
	long code_;
};

inline FASTCGI_DAEMON_API long
http_method_mask::mask() const {
	return mask_;
}

inline FASTCGI_DAEMON_API bool
http_method::operator == (http_method const &other) const {
	return code_ == other.code_;
}

inline FASTCGI_DAEMON_API bool
http_method::operator != (http_method const &other) const {
	return code_ != other.code_;
}

inline FASTCGI_DAEMON_API long
http_method::code() const {
	return code_;
}

inline FASTCGI_DAEMON_API void
swap(http_method_mask &mask, http_method_mask &other) throw () {
	mask.swap(other);
}

inline FASTCGI_DAEMON_API void
swap(http_method &method, http_method &other) throw () {
	method.swap(other);
}

FASTCGI_DAEMON_API http_method_mask
operator | (http_method const &method, http_method const &other);

FASTCGI_DAEMON_API http_method_mask
operator | (http_method_mask const &mask, http_method_mask const &other);

template <typename Char, typename Traits> FASTCGI_DAEMON_API inline std::basic_ostream<Char, Traits>&
operator << (std::basic_ostream<Char, Traits> &out, http_method const &method) {
	out << method.name();
	return out;
}

} // namespace

#endif // FASTCGI_DAEMON_HTTP_METHOD_HPP_INCLUDED
