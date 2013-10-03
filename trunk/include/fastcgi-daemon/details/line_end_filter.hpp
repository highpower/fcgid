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

#ifndef FASTCGI_DAEMON_DETAILS_LINE_END_FILTER_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_LINE_END_FILTER_HPP_INCLUDED

#include <cassert>
#include <iterator>
#include <boost/type_traits.hpp>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/details/range.hpp"
#include "fastcgi-daemon/details/strings.hpp"
#include "fastcgi-daemon/details/functors.hpp"
#include "fastcgi-daemon/details/iterator.hpp"

namespace fcgid { namespace details {

template <typename Iter>
class line_end_filter : public iterator<line_end_filter<Iter>, typename boost::add_const<typename std::iterator_traits<Iter>::value_type>::type, std::bidirectional_iterator_tag> {

public:
	line_end_filter();
	line_end_filter(Iter begin, Iter end);

	typedef line_end_filter<Iter> type;
	typedef typename boost::add_const<typename std::iterator_traits<Iter>::value_type>::type value_type;
	typedef iterator<line_end_filter<Iter>, value_type, std::forward_iterator_tag> base_type;
	typedef typename base_type::reference reference;

	void increment();
	void decrement();
	
	Iter base() const;
	reference dereference() const;
	bool equal(line_end_filter<Iter> const &other) const;

private:
	void seek_forward();
	void seek_reverse();
	static value_type const space;

private:
	Iter i_, begin_, end_;
	bool empty_, filter_triggered_;
	is_space<value_type> space_checker_;
	is_line_end<value_type> line_end_checker_;
};

template <typename Iter> typename line_end_filter<Iter>::value_type const
line_end_filter<Iter>::space = static_cast<typename line_end_filter<Iter>::value_type>(' ');

template <typename Iter> inline
line_end_filter<Iter>::line_end_filter() :
	i_(), begin_(), end_(), empty_(true), filter_triggered_(false), space_checker_(), line_end_checker_()
{
}

template <typename Iter> inline
line_end_filter<Iter>::line_end_filter(Iter begin, Iter end) :
	i_(begin), begin_(begin), end_(end), empty_(false), filter_triggered_(false), space_checker_(), line_end_checker_()
{
	seek_forward();
}

template <typename Iter> inline void
line_end_filter<Iter>::increment() {
	assert(!empty_);
	if (filter_triggered_) {
		filter_triggered_ = false;
	}
	else {
		++i_;
	}
	seek_forward();
}

template <typename Iter> inline void
line_end_filter<Iter>::decrement() {
	assert(!empty_);
	if (filter_triggered_) {
		filter_triggered_ = false;
	}
	else {
		--i_;
	}
	seek_reverse();
}

template <typename Iter> inline Iter
line_end_filter<Iter>::base() const {
	return i_;
}

template <typename Iter> inline typename line_end_filter<Iter>::reference
line_end_filter<Iter>::dereference() const {
	assert(!empty_);
	if (filter_triggered_) {
		return space;
	}
	return *i_;
}

template <typename Iter> inline bool
line_end_filter<Iter>::equal(line_end_filter<Iter> const &other) const {
	if (empty_ && other.empty_) {
		return true;
	}
	return (i_ == other.i_) && (filter_triggered_ == other.filter_triggered_);
}

template <typename Iter> inline void
line_end_filter<Iter>::seek_forward() {
	assert(!filter_triggered_);
	if ((end_ != i_) && line_end_checker_(*i_)) {
		Iter new_line = next_not_matched(i_, end_, line_end_checker_);
		if (end_ != new_line && space_checker_(*new_line)) {
			i_ = next_not_matched(new_line, end_, space_checker_);
			filter_triggered_ = true;
		}
	}
}

template <typename Iter> inline void
line_end_filter<Iter>::seek_reverse() {
	typedef std::reverse_iterator<Iter> iterator_type;
	if ((end_ != i_) && space_checker_(*i_)) {
		iterator_type rend(begin_);
		iterator_type space_end = next_not_matched(iterator_type(i_), rend, space_checker_);
		if (line_end_checker_(*space_end)) {
			i_ = next_not_matched(space_end, rend, line_end_checker_).base();
			--i_;
			filter_triggered_ = true;
		}
	}
}

template <typename Iter> inline range<typename line_end_filter<Iter>::type>
make_line_end_filtered(Iter begin, Iter end) {
	line_end_filter<Iter> filtered_begin(begin, end), filtered_end(end, end);
	return make_range(filtered_begin, filtered_end);
}

template <typename Iter> range<Iter>
make_line_end_unfiltered(line_end_filter<Iter> begin, line_end_filter<Iter> end) {
	return make_range(begin.base(), end.base());
}

template <typename Sequence> inline range<typename line_end_filter<typename Sequence::const_iterator>::type>
make_line_end_filtered(Sequence const &seq) {
	return make_line_end_filtered(seq.begin(), seq.end());
}

template <typename Iter> range<Iter>
make_line_end_unfiltered(range<typename line_end_filter<Iter>::type> const &r) {
	return make_line_end_unfiltered(r.begin(), r.end());
}

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_LINE_END_FILTER_HPP_INCLUDED
