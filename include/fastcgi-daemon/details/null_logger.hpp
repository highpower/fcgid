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

#ifndef FASTCGI_DAEMON_NULL_LOGGER_HPP_INCLUDED
#define FASTCGI_DAEMON_NULL_LOGGER_HPP_INCLUDED

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/logger.hpp"

namespace fcgid {

class null_logger : public logger {

public:
	null_logger();
	virtual ~null_logger();

	virtual void info(char const *format, ...);
	virtual void debug(char const *format, ...);
	virtual void error(char const *format, ...);

private:
	null_logger(null_logger const &);
	null_logger& operator = (null_logger const &);
};

} // namespace

#endif // FASTCGI_DAEMON_LOGGER_HPP_INCLUDED
