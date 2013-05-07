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

#ifndef FASTCGI_DAEMON_DETAILS_POST_BUFFER_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_POST_BUFFER_HPP_INCLUDED

#include <iosfwd>
#include <cassert>
#include <iterator>
#include <boost/type_traits.hpp>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/details/resource.hpp"

namespace fcgid { namespace details {

struct malloc_memory_traits {
	void destroy(char *value);
	static char* default_value();
};

class post_buffer {

public:
	post_buffer();
	
	typedef char value_type;
	typedef char const* iterator;
	typedef char const* const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	typedef boost::make_unsigned<std::iterator_traits<iterator>::difference_type>::type size_type;
	
	bool empty() const;
	size_type size() const;
	const_iterator end() const;
	const_iterator begin() const;

	std::ostream& write(std::ostream &out);
	template <typename Func> std::size_t read(Func func, std::size_t size);
	
private:
	post_buffer(post_buffer const &);
	post_buffer& operator = (post_buffer const &);
	void reserve(std::size_t size);
	
private:
	char *begin_, *end_;
	resource<char*, malloc_memory_traits> buffer_;
};

inline post_buffer::const_iterator
post_buffer::end() const {
	assert(buffer_);
	return end_;
}

inline post_buffer::const_iterator
post_buffer::begin() const {
	assert(buffer_);
	return begin_;
}

template <typename Func> inline std::size_t
post_buffer::read(Func func, std::size_t size) {
	reserve(size);
	return func(buffer_.get(), size);
}

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_POST_BUFFER_HPP_INCLUDED
