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

#ifndef FASTCGI_DAEMON_FILESET_HPP_INCLUDED
#define FASTCGI_DAEMON_FILESET_HPP_INCLUDED

#include "fastcgi-daemon/file.hpp"
#include "fastcgi-daemon/config.hpp"

namespace fcgid {

class FASTCGI_DAEMON_API fileset {

public:
	fileset();
	virtual ~fileset();
	
	virtual void next() = 0;
	virtual bool empty() const = 0;

	virtual std::string const& name() const = 0;
	virtual file value() const = 0;

private:
	fileset(fileset const &) = delete;
	fileset& operator = (fileset const &) = delete;
};

} // namespace

#endif // FASTCGI_DAEMON_FILESET_HPP_INCLUDED

