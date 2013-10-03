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

#ifndef FASTCGI_DAEMON_HTTP_STATUS_HPP_INCLUDED
#define FASTCGI_DAEMON_HTTP_STATUS_HPP_INCLUDED

#include "fastcgi-daemon/config.hpp"

namespace fcgid {

class FASTCGI_DAEMON_API http_status {

public:
	explicit http_status(long code);

	void swap(http_status &other) throw ();
	bool operator == (http_status const &other) const;
	bool operator != (http_status const &other) const;

	long code() const;
	char const* message() const;
	
	bool is_success() const;
	bool is_redirect() const;
	bool is_client_error() const;
	bool is_server_error() const;

	static http_status const ok;
	static http_status const created;
	static http_status const accepted;
	static http_status const non_authoritative;
	static http_status const no_content;
	
	static http_status const moved_permanently;
	static http_status const moved_temporarily;
	static http_status const see_other;
	static http_status const not_modified;

	static http_status const bad_request;
	static http_status const unauthorized;
	static http_status const payment_required;
	static http_status const forbidden;
	static http_status const not_found;
	static http_status const method_not_allowed;
	static http_status const entity_too_large;
	static http_status const range_not_satisfiable;
	
	static http_status const internal_error;
	static http_status const not_implemented;
	static http_status const bad_gateway;
	static http_status const service_unavailable;
	static http_status const gateway_timeout;
	static http_status const version_not_supported;
	static http_status const bad;

private:
	long code_;
};

inline FASTCGI_DAEMON_API long
http_status::code() const {
	return code_;
}

inline FASTCGI_DAEMON_API bool
http_status::operator == (http_status const &other) const {
	return code_ == other.code_;
}

inline FASTCGI_DAEMON_API bool
http_status::operator != (http_status const &other) const {
	return code_ != other.code_;
}

inline FASTCGI_DAEMON_API void
swap(http_status &target, http_status &other) throw () {
	target.swap(other);
}

} // namespace

#endif // FASTCGI_DAEMON_HTTP_STATUS_HPP_INCLUDED
