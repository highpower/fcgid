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

#ifndef FASTCGI_DAEMON_DETAILS_ITERATOR_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_ITERATOR_HPP_INCLUDED

#include <iterator>

#include "fastcgi-daemon/config.hpp"

namespace fcgid { namespace details {

template <typename Derived, typename Type, typename Dist, typename Pointer,
	typename Reference, typename Tag> struct iterator_base;

template <typename Derived, typename Type, typename Dist, typename Pointer, typename Reference>
struct iterator_base<Derived, Type, Dist, Pointer, Reference, std::forward_iterator_tag> {

	Pointer operator -> ();
	Reference operator * ();

	Derived& operator ++ ();
	Derived operator ++ (int);

	bool operator == (Derived const &other) const;
	bool operator != (Derived const &other) const;
};

template <typename Derived, typename Type, typename Dist, typename Pointer, typename Reference>
struct iterator_base<Derived, Type, Dist, Pointer, Reference, std::bidirectional_iterator_tag> :
	public iterator_base<Derived, Type, Dist, Pointer, Reference, std::forward_iterator_tag>
{
	Derived& operator -- ();
	Derived operator -- (int);
};

template <typename Derived, typename Type, typename Dist, typename Pointer, typename Reference>
struct iterator_base<Derived, Type, Dist, Pointer, Reference, std::random_access_iterator_tag> :
	public iterator_base<Derived, Type, Dist, Pointer, Reference, std::bidirectional_iterator_tag>
{
	Derived operator + (Dist dist) const;
	Derived& operator += (Dist dist);
	
	Derived operator - (Dist dist) const;
	Derived& operator -= (Dist dist);
	
	Type operator [] (Dist dist) const;
	Dist operator - (Derived const &other) const;

	bool operator < (Derived const &other) const;
	bool operator > (Derived const &other) const;
	bool operator <= (Derived const &other) const;
	bool operator >= (Derived const &other) const;
};

template <typename Derived, typename Type, typename Tag, typename Dist = std::ptrdiff_t, typename Pointer = Type*, typename Reference = Type&> 
struct iterator : public iterator_base<Derived, Type, Dist, Pointer, Reference, Tag>, public std::iterator<Tag, Type, Dist, Pointer, Reference> {
};

template <typename Derived, typename Type, typename Dist, typename Pointer, typename Reference> inline Derived&
iterator_base<Derived, Type, Dist, Pointer, Reference, std::forward_iterator_tag>::operator ++ () {
	Derived &object = static_cast<Derived&>(*this);
	object.increment();
	return object;
}

template <typename Derived, typename Type, typename Dist, typename Pointer, typename Reference> inline Derived
iterator_base<Derived, Type, Dist, Pointer, Reference, std::forward_iterator_tag>::operator ++ (int) {
	Derived copy(static_cast<Derived const&>(*this));
	++(*this);
	return copy;
}

template <typename Derived, typename Type, typename Dist, typename Pointer, typename Reference> inline Pointer
iterator_base<Derived, Type, Dist, Pointer, Reference, std::forward_iterator_tag>::operator -> () {
	Reference ref = static_cast<Derived*>(this)->dereference();
	return &ref;
}

template <typename Derived, typename Type, typename Dist, typename Pointer, typename Reference> inline Reference
iterator_base<Derived, Type, Dist, Pointer, Reference, std::forward_iterator_tag>::operator * () {
	return static_cast<Derived*>(this)->dereference();
}

template <typename Derived, typename Type, typename Dist, typename Pointer, typename Reference> inline bool
iterator_base<Derived, Type, Dist, Pointer, Reference, std::forward_iterator_tag>::operator == (Derived const &other) const {
	return static_cast<Derived const*>(this)->equal(other);
}

template <typename Derived, typename Type, typename Dist, typename Pointer, typename Reference> inline bool
iterator_base<Derived, Type, Dist, Pointer, Reference, std::forward_iterator_tag>::operator != (Derived const &other) const {
	return !static_cast<Derived const*>(this)->equal(other);
}

template <typename Derived, typename Type, typename Dist, typename Pointer, typename Reference> inline Derived&
iterator_base<Derived, Type, Dist, Pointer, Reference, std::bidirectional_iterator_tag>::operator -- () {
	Derived &object = static_cast<Derived&>(*this);
	object.decrement();
	return object;
}

template <typename Derived, typename Type, typename Dist, typename Pointer, typename Reference> inline Derived
iterator_base<Derived, Type, Dist, Pointer, Reference, std::bidirectional_iterator_tag>::operator -- (int) {
	Derived copy(static_cast<Derived const&>(*this));
	--(*this);
	return copy;
}

template <typename Derived, typename Type, typename Dist, typename Pointer, typename Reference> inline Derived
iterator_base<Derived, Type, Dist, Pointer, Reference, std::random_access_iterator_tag>::operator + (Dist dist) const {
	Derived copy(static_cast<Derived const&>(*this));
	copy += dist;
	return copy;
}

template <typename Derived, typename Type, typename Dist, typename Pointer, typename Reference> inline Derived&
iterator_base<Derived, Type, Dist, Pointer, Reference, std::random_access_iterator_tag>::operator += (Dist dist) {
	Derived &object = static_cast<Derived&>(*this);
	object.advance(dist);
	return object;
}

template <typename Derived, typename Type, typename Dist, typename Pointer, typename Reference> inline Derived
iterator_base<Derived, Type, Dist, Pointer, Reference, std::random_access_iterator_tag>::operator - (Dist dist) const {
	Derived copy(static_cast<Derived const&>(*this));
	copy -= dist;
	return copy;
}

template <typename Derived, typename Type, typename Dist, typename Pointer, typename Reference> inline Derived&
iterator_base<Derived, Type, Dist, Pointer, Reference, std::random_access_iterator_tag>::operator -= (Dist dist) {
	Derived &object = static_cast<Derived&>(*this);
	object.advance(-dist);
	return object;
}

template <typename Derived, typename Type, typename Dist, typename Pointer, typename Reference> inline Type
iterator_base<Derived, Type, Dist, Pointer, Reference, std::random_access_iterator_tag>::operator [] (Dist dist) const {
	Derived copy(static_cast<Derived const&>(*this));
	copy += dist;
	return copy.dereference();
}

template <typename Derived, typename Type, typename Dist, typename Pointer, typename Reference> inline Dist
iterator_base<Derived, Type, Dist, Pointer, Reference, std::random_access_iterator_tag>::operator - (Derived const &other) const {
	return other.distance(static_cast<Derived const&>(*this));
}

template <typename Derived, typename Type, typename Dist, typename Pointer, typename Reference> inline bool
iterator_base<Derived, Type, Dist, Pointer, Reference, std::random_access_iterator_tag>::operator < (Derived const &other) const {
	return (static_cast<Derived const&>(*this) - other) < 0;
}

template <typename Derived, typename Type, typename Dist, typename Pointer, typename Reference> inline bool
iterator_base<Derived, Type, Dist, Pointer, Reference, std::random_access_iterator_tag>::operator > (Derived const &other) const {
	return (static_cast<Derived const&>(*this) - other) > 0;
}

template <typename Derived, typename Type, typename Dist, typename Pointer, typename Reference> inline bool
iterator_base<Derived, Type, Dist, Pointer, Reference, std::random_access_iterator_tag>::operator <= (Derived const &other) const {
	return (static_cast<Derived const&>(*this) - other) <= 0;
}

template <typename Derived, typename Type, typename Dist, typename Pointer, typename Reference> inline bool
iterator_base<Derived, Type, Dist, Pointer, Reference, std::random_access_iterator_tag>::operator >= (Derived const &other) const {
	return (static_cast<Derived const&>(*this) - other) >= 0;
}

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_ITERATOR_HPP_INCLUDED
