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

#ifndef FASTCGI_DAEMON_LOGGER_HPP_INCLUDED
#define FASTCGI_DAEMON_LOGGER_HPP_INCLUDED

#include "fastcgi-daemon/config.hpp"

namespace fcgid {

class FASTCGI_DAEMON_API logger {

public:
	logger();
	virtual ~logger();

	virtual void info(char const *format, ...) = 0;
	virtual void debug(char const *format, ...) = 0;
	virtual void error(char const *format, ...) = 0;

private:
	logger(logger const &) = delete;
	logger& operator = (logger const &) = delete;
};

} // namespace

#endif // FASTCGI_DAEMON_LOGGER_HPP_INCLUDED
