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

#ifndef FASTCGI_DAEMON_DETAILS_RESOURCE_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_RESOURCE_HPP_INCLUDED

#include <algorithm>

#include "fastcgi-daemon/config.hpp"

namespace fcgid { namespace details {

template <typename Type, typename Traits>
class resource {

public:
	resource();
	explicit resource(Type value, Traits const &traits = Traits());
	virtual ~resource();
	
	Type release();
	void reset(Type value);
	
	Type get();
	Type const get() const;

	class bool_convertible;
	operator bool_convertible const* () const;
	
private:
	resource(resource const &);
	resource& operator = (resource const &);
	void swap(resource<Type, Traits> &other) throw ();
	
private:
	Type value_;
	Traits traits_;
};

template <typename Type, typename Traits> inline
resource<Type, Traits>::resource() :
	value_(Traits::default_value()), traits_()
{
}

template <typename Type, typename Traits> inline 
resource<Type, Traits>::resource(Type value, Traits const &traits) :
	value_(value), traits_(traits)
{
}

template <typename Type, typename Traits> inline 
resource<Type, Traits>::~resource() {
	if (Traits::default_value() != value_) {
		traits_.destroy(value_);
	}
}

template <typename Type, typename Traits> inline Type
resource<Type, Traits>::release() {
	using std::swap;
	Type result = Traits::default_value();
	swap(value_, result);
	return result;
}

template <typename Type, typename Traits> inline void
resource<Type, Traits>::reset(Type value) {
	resource<Type, Traits> tmp(value, traits_);
	swap(tmp);
}

template <typename Type, typename Traits> inline Type
resource<Type, Traits>::get() {
	return value_;
}

template <typename Type, typename Traits> inline Type const
resource<Type, Traits>::get() const {
	return value_;
}

template <typename Type, typename Traits> inline
resource<Type, Traits>::operator typename resource<Type, Traits>::bool_convertible const* () const {
	return Traits::default_value() != value_ ? reinterpret_cast<bool_convertible const*>(this) : 0;
}

template <typename Type, typename Traits> inline void
resource<Type, Traits>::swap(resource<Type, Traits> &other) throw () {
	using std::swap;
	swap(value_, other.value_);
}

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_RESOURCE_HPP_INCLUDED
