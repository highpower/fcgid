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

#ifndef FASTCGI_DAEMON_FASTCGI_ITER_STREAMBUF_HPP_INCLUDED
#define FASTCGI_DAEMON_FASTCGI_ITER_STREAMBUF_HPP_INCLUDED

#include <limits>
#include <cassert>
#include <cstring>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <boost/static_assert.hpp>

#include "fastcgi-daemon/config.hpp"

namespace fcgid { namespace details {

template <typename Iter, typename Traits = std::char_traits<typename std::iterator_traits<Iter>::value_type> >
class iter_streambuf : public std::basic_streambuf<typename std::iterator_traits<Iter>::value_type, Traits> {

public:
	iter_streambuf(Iter begin, Iter end);
	virtual ~iter_streambuf();

	typedef typename std::iterator_traits<Iter>::value_type char_type;
	typedef typename std::iterator_traits<Iter>::difference_type difference_type;
	typedef std::basic_streambuf<char_type, Traits> base_type;
	
	typedef typename base_type::int_type int_type;
	typedef typename base_type::pos_type pos_type;
	typedef typename base_type::off_type off_type;
	
	virtual int_type underflow();
	virtual int_type pbackfail(int_type value);

	virtual pos_type seekpos(pos_type pos, std::ios::openmode which);
	virtual pos_type seekoff(off_type off, std::ios::seekdir dir, std::ios::openmode which);

	virtual std::streamsize showmanyc();

private:
	iter_streambuf(iter_streambuf const &);
	iter_streambuf& operator = (iter_streambuf const &);

	int_type fill();
	using base_type::gptr;
	using base_type::setg;
	using base_type::eback;
	using base_type::egptr;
	
	static pos_type const bad_position;
	static std::size_t const buffer_size = 4096;

	BOOST_STATIC_ASSERT(std::numeric_limits<off_type>::is_signed);
	BOOST_STATIC_ASSERT(std::numeric_limits<difference_type>::is_signed);
		
private:
	Iter current_;
	Iter const begin_, end_;
	char_type buffer_[buffer_size];
};

template <typename Iter, typename Traits> typename iter_streambuf<Iter, Traits>::pos_type const
iter_streambuf<Iter, Traits>::bad_position(static_cast<std::streamoff>(-1));

template <typename Iter, typename Traits> std::size_t const
iter_streambuf<Iter, Traits>::buffer_size;

template <typename Iter, typename Traits> inline
iter_streambuf<Iter, Traits>::iter_streambuf(Iter begin, Iter end) :
	base_type(), current_(begin), begin_(begin), end_(end)
{
	setg(buffer_, buffer_, buffer_);
}

template <typename Iter, typename Traits> inline
iter_streambuf<Iter, Traits>::~iter_streambuf() {
}

template <typename Iter, typename Traits> inline typename iter_streambuf<Iter, Traits>::int_type
iter_streambuf<Iter, Traits>::underflow() {
	assert(gptr() == egptr());
	if (end_ == current_) {
		return Traits::eof();
	}
	return fill();
}

template <typename Iter, typename Traits> inline typename iter_streambuf<Iter, Traits>::int_type
iter_streambuf<Iter, Traits>::pbackfail(typename iter_streambuf<Iter, Traits>::int_type value) {
	assert(gptr() == eback());
	if (begin_ == current_) {
		return Traits::eof();
	}
	--current_;
	difference_type dist = static_cast<difference_type>(egptr() - eback());
	memmove(buffer_ + 1, buffer_, dist - 1);
	buffer_[0] = value;
	return Traits::to_int_type(value);
}

template <typename Iter, typename Traits> inline typename iter_streambuf<Iter, Traits>::pos_type
iter_streambuf<Iter, Traits>::seekpos(typename iter_streambuf<Iter, Traits>::pos_type pos, std::ios::openmode which) {
	return seekoff(static_cast<off_type>(pos), std::ios::beg, which);
}

template <typename Iter, typename Traits> inline typename iter_streambuf<Iter, Traits>::pos_type
iter_streambuf<Iter, Traits>::seekoff(typename iter_streambuf<Iter, Traits>::off_type off, std::ios::seekdir dir, std::ios::openmode which) {
	
	if (!(which & std::ios::in)) {
		return bad_position;
	}		
	
	difference_type size = std::distance(begin_, end_);
	difference_type dist = static_cast<difference_type>(off);
	difference_type position = static_cast<difference_type>(-1);
	
	if ((std::ios::beg == dir) && (dist >= 0) && (dist < size)) {
		position = dist;
	}
	else if ((std::ios::end == dir) && (dist >= 0) && (dist < size)) {
		position = size - dist;
	}
	else if (std::ios::cur == dir) {
		position = std::distance(begin_, current_) - static_cast<difference_type>(egptr() - gptr()) + dist;
		if ((position < 0) || (position >= size)) {
			return bad_position;
		}
	}
	if (static_cast<difference_type>(-1) != position) {
		current_ = begin_;
		std::advance(current_, position);
		fill();
		return static_cast<pos_type>(position);
	}
	return bad_position;
}

template <typename Iter, typename Traits> inline typename iter_streambuf<Iter, Traits>::int_type
iter_streambuf<Iter, Traits>::fill() {
	std::size_t size = 0;
	for (; end_ != current_ && size < buffer_size; ++current_, ++size) {
		buffer_[size] = *current_;
	}
	setg(buffer_, buffer_, buffer_ + size);
	return Traits::to_int_type(buffer_[0]);
}

template <typename Iter, typename Traits> inline std::streamsize
iter_streambuf<Iter, Traits>::showmanyc() {
	assert(gptr() == egptr());
	difference_type dist = std::distance(current_, end_);
	return (0 == dist) ? static_cast<std::streamsize>(-1) : static_cast<std::streamsize>(dist);
}

}} // namespaces

#endif // FASTCGI_DAEMON_FASTCGI_ITER_STREAMBUF_HPP_INCLUDED
