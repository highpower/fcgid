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

#ifndef FASTCGI_DAEMON_DETAILS_ITER_FILE_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_ITER_FILE_HPP_INCLUDED

#include <cassert>
#include <fstream>
#include <iomanip>
#include <boost/static_assert.hpp>

#include "fastcgi-daemon/file.hpp"
#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/details/range.hpp"
#include "fastcgi-daemon/details/file_impl.hpp"
#include "fastcgi-daemon/details/iter_streambuf.hpp"

namespace fcgid { namespace details {

template <typename Iter, typename A>
class iter_file : public file_impl<A> {

public:
	typedef typename file_impl<A>::string_type string_type;
	iter_file(string_type const &name, string_type const &type, range<Iter> const &cont);
	virtual ~iter_file();

	virtual std::streamsize size() const;
	virtual string_type const& name() const;
	virtual string_type const& content_type() const;

	virtual std::istream& stream();
	virtual void save(char const *name) const;

private:
	iter_file(iter_file const &);
	iter_file& operator = (iter_file const &);

private:
	range<Iter> content_;
	string_type name_, type_;
	iter_streambuf<Iter> buffer_;
	std::istream stream_;
};

template <typename Iter, typename A> inline
iter_file<Iter, A>::iter_file(typename iter_file<Iter, A>::string_type const &name, typename iter_file<Iter, A>::string_type const &type, range<Iter> const &cont) :
	file_impl<A>(), content_(cont), name_(name), type_(type), buffer_(content_.begin(), content_.end()), stream_(&buffer_)
{
}

template <typename Iter, typename A> inline 
iter_file<Iter, A>::~iter_file() {
}

template <typename Iter, typename A> inline std::streamsize
iter_file<Iter, A>::size() const {
	BOOST_STATIC_ASSERT(sizeof(std::streamsize) >= sizeof(typename range<Iter>::size_type));
	return static_cast<std::streamsize>(content_.size());
}

template <typename Iter, typename A> inline typename iter_file<Iter, A>::string_type const&
iter_file<Iter, A>::name() const {
	return name_;
}

template <typename Iter, typename A> inline typename iter_file<Iter, A>::string_type const&
iter_file<Iter, A>::content_type() const {
	return type_;
}

template <typename Iter, typename A> inline std::istream&
iter_file<Iter, A>::stream() {
	return stream_;
}

template <typename Iter, typename A> inline void
iter_file<Iter, A>::save(char const *name) const {
	std::ofstream file(name);
	file.exceptions(std::ios::badbit | std::ios::failbit);
	std::copy(content_.begin(), content_.end(), std::ostream_iterator<typename std::iterator_traits<Iter>::value_type>(file));
}

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_ITER_FILE_HPP_INCLUDED
