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

#ifndef FASTCGI_DAEMON_FILE_HPP_INCLUDED
#define FASTCGI_DAEMON_FILE_HPP_INCLUDED

#include <string>
#include <iosfwd>
#include <boost/shared_ptr.hpp>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/forward.hpp"

namespace fcgid {

class FASTCGI_DAEMON_API file {

public:
	typedef details::file_impl<stdalloc> impl_type;

	file();
	explicit file(boost::shared_ptr<impl_type> const &impl);
	virtual ~file();	
	
	file(file const &other);
	file& operator = (file const &other);
	void swap(file &other) throw ();
	
	std::streamsize size() const;
	std::string const& name() const;
	std::string const& content_type() const;

	std::istream& stream();
	void save(char const *name) const;
	void save(std::string const &name) const;

	operator std::istream& ();

private:
	boost::shared_ptr<impl_type> impl_;
};

inline FASTCGI_DAEMON_API
file::operator std::istream& () {
	return stream();
}

inline FASTCGI_DAEMON_API void
swap(file &target, file &other) throw () {
	target.swap(other);
}

} // namespace

#endif // FASTCGI_DAEMON_FILE_HPP_INCLUDED
