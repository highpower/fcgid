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

#ifndef FASTCGI_DAEMON_DETAILS_STRINGS_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_STRINGS_HPP_INCLUDED

#include <utility>
#include <type_traits>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/details/range.hpp"
#include "fastcgi-daemon/details/functors.hpp"

namespace fcgid { namespace details {

template <typename Iter, typename Pred> inline Iter
next_matched(Iter begin, Iter end, Pred pred) {
	return std::find_if(begin, end, pred);
}

template <typename Iter, typename Pred> inline Iter
next_not_matched(Iter begin, Iter end, Pred pred) {
	for (Iter i = begin; i != end; ++i) {
		if (!pred(*i)) return i;
	}
	return end;
}

template <typename Sequence, typename Pred> inline Sequence
trim_left_if(Sequence const &seq, Pred pred) {
    return Sequence(next_not_matched(seq.begin(), seq.end(), pred), seq.end());
}

template <typename Sequence> inline Sequence
trim_left(Sequence const &seq) {
	return trim_left_if(seq, is_space<typename Sequence::value_type>());
}

template <typename Sequence> inline Sequence
trim_chars_left(Sequence const &seq, typename Sequence::value_type val) {
	return trim_left_if(seq, is_equal<typename Sequence::value_type>(val));
}

template <typename Sequence, typename Pred> inline Sequence
trim_right_if(Sequence const &seq, Pred pred) {
	return Sequence(seq.begin(), next_not_matched(seq.rbegin(), seq.rend(), pred).base());
}

template <typename Sequence> inline Sequence
trim_right(Sequence const &seq) {
    return trim_right_if(seq, is_space<typename Sequence::value_type>());
}

template <typename Sequence> inline Sequence
trim_chars_right(Sequence const &seq, typename Sequence::value_type val) {
	return trim_right_if(seq, is_equal<typename Sequence::value_type>(val));
}

template <typename Sequence, typename Pred> inline Sequence
trim_if(Sequence const &seq, Pred pred) {
	return trim_left_if(trim_right_if(seq, pred), pred);
}

template <typename Sequence> inline Sequence
trim(Sequence const &seq) {
	return trim_left(trim_right(seq));
}

template <typename Sequence> inline Sequence
trim_chars(Sequence const &seq, typename Sequence::value_type value) {
	return trim_chars_left(trim_chars_right(seq, value), value);
}

template <typename Sequence> inline Sequence
truncate_left(Sequence const &seq, typename Sequence::size_type n) {
	typename Sequence::const_iterator begin = seq.begin(), end = seq.end();
	while (begin != end && n--) {
		++begin;
	}
	return Sequence(begin, end);
}

template <typename Sequence> inline Sequence
truncate_right(Sequence const &seq, typename Sequence::size_type n) {
	typename Sequence::const_iterator begin = seq.begin(), end = seq.end();
	while (begin != end && n--) {
		--end;
	}
	return Sequence(begin, end);
}

template <typename Sequence> inline Sequence
truncate(Sequence const &seq, typename Sequence::size_type left, typename Sequence::size_type right) {
	return truncate_left(truncate_right(seq, right), left);
}

template <typename Sequence> inline bool
split_impl(Sequence const &seq, typename Sequence::const_iterator pos, 
	typename Sequence::size_type size, Sequence &first, Sequence &second) 
{
	typename Sequence::const_iterator begin = seq.begin(), end = seq.end();
	first = Sequence(begin, pos);
	if (pos != end) {
		typename Sequence::const_iterator start = pos;
		std::advance(start, size);
		second = Sequence(start, end);
		return true;
	}
	else {
		second = Sequence(end, end);
		return false;
	}
}

template <typename Sequence, typename Func> inline bool
split_if_once(Sequence const &seq, Func func, Sequence &first, Sequence &second) {
	return split_impl(seq, std::find_if(seq.begin(), seq.end(), func), 1, first, second);
}

template <typename Sequence> inline bool
split_once(Sequence const &seq, typename Sequence::value_type c, Sequence &first, Sequence &second) {
	return split_impl(seq, std::find(seq.begin(), seq.end(), c), 1, first, second);
}

template <typename Sequence, typename Delim> inline bool
split_once(Sequence const &seq, Delim const &delim, Sequence &first, Sequence &second, 
	typename Delim::value_type const *sfinae = 0) 
{
	(void) sfinae;
	return split_impl(seq, std::search(seq.begin(), seq.end(), delim.begin(), delim.end()), delim.size(), first, second);
}

template <typename Sequence> inline bool
split_once(Sequence const &seq, typename Sequence::value_type const *chars, Sequence &first, Sequence &second) {
	return split_once(seq, make_range(chars), first, second, static_cast<range<char const*>::value_type const*>(0));
}

template <typename Sequence, typename Delim> inline bool
split_first_of_once(Sequence const &seq, Delim const &delim, Sequence &first, Sequence &second, 
	typename Delim::value_type const *sfinae = 0) 
{
	(void) sfinae;
	return split_impl(seq, std::find_first_of(seq.begin(), seq.end(), delim.begin(), delim.end()), 1, first, second);
}

template <typename Sequence> inline bool
split_first_of_once(Sequence const &seq, typename Sequence::value_type const *chars, Sequence &first, Sequence &second) {
	return split_first_of_once(seq, make_range(chars), first, second, static_cast<range<char const*>::value_type const*>(0));
}

template <typename Sequence, typename Prefix> inline bool
starts_with(Sequence const &seq, Prefix const &prefix, typename Prefix::value_type const *sfinae = 0) {
	(void) sfinae;
	typedef typename std::common_type<typename Sequence::size_type, typename Prefix::size_type>::type size_type;
	if (static_cast<size_type>(prefix.size()) <= static_cast<size_type>(seq.size())) {
		return std::equal(prefix.begin(), prefix.end(), seq.begin());
	}
	return false;
}

template <typename Sequence> inline bool
starts_with(Sequence const &seq, char const *prefix) {
	return starts_with(seq, make_range(prefix), static_cast<range<char const*>::value_type const*>(0));
}

template <typename Prefix> inline bool
starts_with(char const *string, Prefix const &prefix) {
	return starts_with(make_range(string), prefix, static_cast<typename Prefix::value_type const*>(0));
}

template <typename Sequence, typename Suffix> inline bool
ends_with(Sequence const &seq, Suffix const &suffix, typename Suffix::value_type const *sfinae = 0) {
	(void) sfinae;
	typedef typename std::common_type<typename Sequence::size_type, typename Suffix::size_type>::type size_type;
	if (static_cast<size_type>(suffix.size()) <= static_cast<size_type>(seq.size())) {
		typename Sequence::const_iterator i = seq.begin();
		std::advance(i, seq.size() - suffix.size());
 		return std::equal(suffix.begin(), suffix.end(), i);
	}
	return false;
}

template <typename Sequence> inline bool
ends_with(Sequence const &seq, char const *suffix) {
	return ends_with(seq, make_range(suffix), static_cast<range<char const*>::value_type const*>(0));
}

template <typename Suffix> inline bool
ends_with(char const *string, Suffix const &suffix) {
	return ends_with(make_range(string), suffix, static_cast<typename Suffix::value_type const*>(0));
}

template <typename Sequence> inline std::pair<Sequence, Sequence>
split_key_value(Sequence const &seq) {
	Sequence head, tail;
	split_once(seq, '=', head, tail);
	return std::make_pair(trim(head), trim(tail));
}

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_STRINGS_HPP_INCLUDED
