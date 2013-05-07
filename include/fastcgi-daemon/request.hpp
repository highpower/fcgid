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

#ifndef FASTCGI_DAEMON_REQUEST_HPP_INCLUDED
#define FASTCGI_DAEMON_REQUEST_HPP_INCLUDED

#include <string>
#include <utility>
#include <boost/shared_ptr.hpp>

#include "fastcgi-daemon/file.hpp"
#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/fileset.hpp"
#include "fastcgi-daemon/paramset.hpp"
#include "fastcgi-daemon/http_method.hpp"

namespace fcgid {

class FASTCGI_DAEMON_API request {

public:
	request();
	virtual ~request();

	virtual bool secure() const = 0;
	virtual http_method method() const = 0;
	virtual std::string const& path_info() const = 0;

	virtual boost::shared_ptr<paramset> vars() const = 0;
	virtual bool has_var(std::string const &name) const = 0;
	virtual std::string const& var(std::string const &name) const = 0;
	
	virtual boost::shared_ptr<paramset> cookies() const = 0;
	virtual bool has_cookie(std::string const &name) const = 0;
	virtual std::string const& cookie(std::string const &name) const = 0;

	virtual boost::shared_ptr<paramset> args() const = 0;
	virtual bool has_arg(std::string const &name) const = 0;
	virtual std::string const& arg(std::string const &name) const = 0;

	virtual boost::shared_ptr<fileset> files() const = 0;
	virtual bool has_file(std::string const &name) const = 0;
	virtual file get_file(std::string const &name) const = 0;
	
	virtual bool has_header(std::string const &name) const = 0;
	virtual std::string const& header(std::string const &name) const = 0;

	virtual void store(char const *file) = 0;
	
private:
	request(request const &);
	request& operator = (request const &);
};

} // namespace

#endif // FASTCGI_DAEMON_REQUEST_HPP_INCLUDED
