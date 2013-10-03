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

#ifndef FASTCGI_DAEMON_DISTINCT_HPP_INCLUDED
#define FASTCGI_DAEMON_DISTINCT_HPP_INCLUDED

#include "fastcgi-daemon/config.hpp"

namespace fcgid {

template <typename Type, typename Tag>
class FASTCGI_DAEMON_API distinct {

public:
	explicit distinct(Type const &value);
	Type const& get() const;

private:
	Type value_;
};

template <typename Type, typename Tag> inline
distinct<Type, Tag>::distinct(Type const &value) :
	value_(value)
{
}

template <typename Type, typename Tag> inline Type const&
distinct<Type, Tag>::get() const {
	return value_;
}

} // namespace

#endif // FASTCGI_DAEMON_DISTINCT_HPP_INCLUDED
