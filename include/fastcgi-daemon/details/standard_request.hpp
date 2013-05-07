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

#ifndef FASTCGI_DAEMON_DETAILS_STANDARD_REQUEST_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_STANDARD_REQUEST_HPP_INCLUDED

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/forward.hpp"
#include "fastcgi-daemon/request.hpp"
#include "fastcgi-daemon/details/fileset_impl.hpp"
#include "fastcgi-daemon/details/paramset_impl.hpp"
#include "fastcgi-daemon/details/generic_request.hpp"

namespace fcgid { namespace details {

template <typename IO>
class standard_request : public request, public generic_request<IO, stdalloc> {

public:
	typedef IO io_type;
	typedef standard_request<IO> type;
	typedef generic_request<IO, stdalloc> base_type;

	standard_request(io_type &io);
	virtual ~standard_request();

	virtual bool secure() const;
	virtual http_method method() const;
	virtual std::string const& path_info() const;

	virtual boost::shared_ptr<paramset> vars() const;
	virtual bool has_var(std::string const &name) const;
	virtual std::string const& var(std::string const &name) const;
	
	virtual boost::shared_ptr<paramset> cookies() const;
	virtual bool has_cookie(std::string const &name) const;
	virtual std::string const& cookie(std::string const &name) const;

	virtual boost::shared_ptr<paramset> args() const;
	virtual bool has_arg(std::string const &name) const;
	virtual std::string const& arg(std::string const &name) const;

	virtual boost::shared_ptr<fileset> files() const;
	virtual bool has_file(std::string const &name) const;
	virtual file get_file(std::string const &name) const;
	
	virtual bool has_header(std::string const &name) const;
	virtual std::string const& header(std::string const &name) const;

	virtual void store(char const *file);
	
private:
	standard_request(standard_request const &);
	standard_request& operator = (standard_request const &);
};


template <typename IO> inline
standard_request<IO>::standard_request(typename standard_request<IO>::io_type &io) :
	base_type(io)
{
}

template <typename IO> inline 
standard_request<IO>::~standard_request() {
}

template <typename IO> inline bool
standard_request<IO>::secure() const {
	return base_type::secure();
}

template <typename IO> inline http_method
standard_request<IO>::method() const {
	return base_type::method();
}

template <typename IO> inline std::string const&
standard_request<IO>::path_info() const {
	return base_type::path_info();
}

template <typename IO> inline boost::shared_ptr<paramset>
standard_request<IO>::vars() const {
	typename base_type::string_map_type const &m = base_type::vars();
	return make_paramset(m.begin(), m.end());
}

template <typename IO> inline bool
standard_request<IO>::has_var(std::string const &name) const {
	return base_type::has_var(name);
}

template <typename IO> inline std::string const&
standard_request<IO>::var(std::string const &name) const {
	return base_type::var(name);
}

template <typename IO> inline boost::shared_ptr<paramset>
standard_request<IO>::cookies() const {
	typename base_type::string_map_type const &m = base_type::cookies();
	return make_paramset(m.begin(), m.end());
}

template <typename IO> inline bool
standard_request<IO>::has_cookie(std::string const &name) const {
	return base_type::has_cookie(name);
}

template <typename IO> inline std::string const&
standard_request<IO>::cookie(std::string const &name) const {
	return base_type::cookie(name);
}

template <typename IO> inline boost::shared_ptr<paramset>
standard_request<IO>::args() const {
	typename base_type::string_multimap_type const &m = base_type::args();
	return make_paramset(m.begin(), m.end());
}

template <typename IO> inline bool
standard_request<IO>::has_arg(std::string const &name) const {
	return base_type::has_arg(name);
}

template <typename IO> inline std::string const&
standard_request<IO>::arg(std::string const &name) const {
	return base_type::arg(name);
}

template <typename IO> inline boost::shared_ptr<fileset>
standard_request<IO>::files() const {
	typename base_type::file_map_type const &m = base_type::files();
	return make_fileset(m.begin(), m.end());
}

template <typename IO> inline bool
standard_request<IO>::has_file(std::string const &name) const {
	return base_type::has_file(name);
}

template <typename IO> inline file
standard_request<IO>::get_file(std::string const &name) const {
	return file(base_type::get_file(name));
}

template <typename IO> inline bool
standard_request<IO>::has_header(std::string const &name) const {
	return base_type::has_header(name);
}

template <typename IO> inline std::string const&
standard_request<IO>::header(std::string const &name) const {
	return base_type::header(name);
}

template <typename IO> inline void
standard_request<IO>::store(char const *file) {
	base_type::store(file);
}

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_STANDARD_REQUEST_HPP_INCLUDED
