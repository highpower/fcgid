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

#ifndef FASTCGI_DAEMON_CONFIG_HPP_INCLUDED
#define FASTCGI_DAEMON_CONFIG_HPP_INCLUDED

#if defined _WIN32 || defined __CYGWIN__

#ifdef FASTCGI_DAEMON_LIBRARY

#ifdef __GNUC__
#define FASTCGI_DAEMON_API __attribute__ ((dllexport))
#else
#define FASTCGI_DAEMON_API __declspec(dllexport)
#endif

#else // FASTCGI_DAEMON_LIBRARY

#ifdef __GNUC__
#define FASTCGI_DAEMON_API __attribute__ ((dllimport))
#else
#define FASTCGI_DAEMON_API __declspec(dllimport)
#endif

#endif // FASTCGI_DAEMON_LIBRARY
#define FASTCGI_DAEMON_LOCAL

#else // defined _WIN32 || defined __CYGWIN__

#if __GNUC__ >= 4
#define FASTCGI_DAEMON_API __attribute__ ((visibility ("default")))
#define FASTCGI_DAEMON_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define FASTCGI_DAEMON_API
#define FASTCGI_DAEMON_LOCAL
#endif

#endif // defined _WIN32 || defined __CYGWIN__

#include <memory>

namespace fcgid {

typedef std::allocator<char> stdalloc;

} // namespace

#endif // FASTCGI_DAEMON_CONFIG_HPP_INCLUDED
