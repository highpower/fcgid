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

#ifndef FASTCGI_DAEMON_DETAILS_URLENCODE_IMPL_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_URLENCODE_IMPL_HPP_INCLUDED

#include <iterator>
#include <boost/static_assert.hpp>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/details/functors.hpp"

namespace fcgid { namespace details {

template <typename Result, typename Range> inline Result
urlencode_impl(Range const &range, Result &result) {

	BOOST_STATIC_ASSERT(sizeof(typename Range::value_type) == 1);
	result.reserve(3 * range.size());

	for (typename Range::const_iterator i = range.begin(), end = range.end(); i != end; ++i) {
		if (char_traits<typename Range::value_type>::is_alpha_numeric(*i)) {
			result.push_back(*i);
			continue;
		}
		switch (*i) {
		case static_cast<typename Range::value_type>('-'):
		case static_cast<typename Range::value_type>('_'):
		case static_cast<typename Range::value_type>('.'):
		case static_cast<typename Range::value_type>('!'):
		case static_cast<typename Range::value_type>('~'):
		case static_cast<typename Range::value_type>('*'):
		case static_cast<typename Range::value_type>('('):
		case static_cast<typename Range::value_type>(')'):
		case static_cast<typename Range::value_type>('\''):
			result.push_back(*i);
			break;
		default:
			typename Range::value_type bytes[3] = { '%', (*i & 0xF0) / 16, *i & 0x0F };
			for (std::size_t i = 1; i < sizeof(bytes); ++i) {
				bytes[i] += (bytes[i] > 9) ? 'A' - 10 : '0';
			}
			result.insert(result.end(), bytes, bytes + sizeof(bytes));
			break;
		}
	}
	return result;
}

template <typename Result, typename Range> inline Result
urlencode(Range const &range) {
	Result result;
	return urlencode_impl(range, result);
}

template <typename Result, typename Range> inline Result
urlencode(Range const &range, Result const &pattern) {
	Result result(pattern);
	return urlencode_impl(range, result);
}

template <typename Result, typename Range> inline Result
urldecode_impl(Range const &range, Result &result) {

	BOOST_STATIC_ASSERT(sizeof(typename Range::value_type) == 1);
	result.reserve(range.size());

	for (typename Range::const_iterator i = range.begin(), end = range.end(); i != end; ++i) {
		switch (*i) {
		case static_cast<typename Range::value_type>('+'):
			result.push_back(static_cast<typename Range::value_type>(' '));
			break;
		case static_cast<typename Range::value_type>('%'):
			if (std::distance(i, end) > 2) {
				typename Range::const_iterator f = i, s = i;
				std::advance(f, 1);
				std::advance(s, 2);
				typename Range::value_type first = *f, second = *s;
				int digit = (first >= 'A' ? ((first & 0xDF) - 'A') + 10 : (first - '0')) * 16;
				digit += (second >= 'A') ? ((second & 0xDF) - 'A') + 10 : (second - '0');
				result.push_back(static_cast<typename Range::value_type>(digit));
				std::advance(i, 2);
			}
			else {
				result.push_back('%');
			}
			break;
		default:
			result.push_back(*i);
			break;
		}
	}
	return result;
}

template <typename Result, typename Range> inline Result
urldecode(Range const &range) {
	Result result;
	return urldecode_impl(range, result);
}

template <typename Result, typename Range> inline Result
urldecode(Range const &range, Result const &pattern) {
	Result result(pattern);
	return urldecode_impl(range, result);
}

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_URLENCODE_IMPL_HPP_INCLUDED
