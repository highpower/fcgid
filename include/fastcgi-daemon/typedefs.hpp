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

#ifndef FASTCGI_DAEMON_TYPEDEFS_HPP_INCLUDED
#define FASTCGI_DAEMON_TYPEDEFS_HPP_INCLUDED

#include <cstddef>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/distinct.hpp"

namespace fcgid {

struct url_tag;
typedef distinct<char const*, url_tag> url_type;

struct backlog_tag;
typedef distinct<unsigned long, backlog_tag> backlog_type;

struct queue_name_tag;
typedef distinct<char const*, queue_name_tag> queue_name_type;

struct thread_count_tag;
typedef distinct<std::size_t, thread_count_tag> thread_count_type;

} // namespaces

#endif // FASTCGI_DAEMON_TYPEDEFS_HPP_INCLUDED

