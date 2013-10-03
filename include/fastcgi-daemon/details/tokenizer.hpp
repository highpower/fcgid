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

#ifndef FASTCGI_DAEMON_DETAILS_TOKENIZER_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_TOKENIZER_HPP_INCLUDED

#include <iterator>
#include <functional>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/details/range.hpp"
#include "fastcgi-daemon/details/strings.hpp"
#include "fastcgi-daemon/details/iterator.hpp"

namespace fcgid { namespace details {

template <typename Pred>
class split_if_match {

public:
	split_if_match(Pred const &pred);
	template <typename Range> bool operator () (Range &head, Range &tail) const;
	typedef split_if_match<Pred> type;

private:
	Pred pred_;
};

template <typename Char>
class split_if_equal {

public:
	split_if_equal(Char value);
	template <typename Range> bool operator () (Range &head, Range &tail) const;
	typedef split_if_equal<Char> type;

private:
	Char value_;
};

template <typename Char>
class split_first_of {

public:
	split_first_of(Char const *charset);
	template <typename Range> bool operator () (Range &head, Range &tail) const;
	typedef split_first_of<Char> type;
	
private:
	Char const *charset_;
};

template <typename Sequence, typename Pred>
class token_iterator : public iterator<token_iterator<Sequence, Pred>, Sequence const, std::forward_iterator_tag> {

public:
	token_iterator(Pred const &pred);
	token_iterator(range<typename Sequence::const_iterator> const &range, Pred const &pred);

	void increment();
	Sequence const& dereference() const;
	bool equal(token_iterator<Sequence, Pred> const &other) const;

private:
	Pred pred_;
	Sequence mutable sequence_;
	range<typename Sequence::const_iterator> head_, tail_;
};

template <typename Sequence, typename Pred>
class tokenizer {

public:
	tokenizer(Sequence const &sequence, Pred const &pred);

	typedef token_iterator<Sequence, Pred> iterator;
	typedef token_iterator<Sequence, Pred> const_iterator;
	typedef typename std::iterator_traits<iterator>::value_type value_type;

	const_iterator end() const;
	const_iterator begin() const;

private:
	Pred pred_;
	Sequence sequence_;
};

template <typename Pred> inline 
split_if_match<Pred>::split_if_match(Pred const &pred) :
	pred_(pred)
{
}

template <typename Pred> template <typename Sequence> inline bool
split_if_match<Pred>::operator () (Sequence &head, Sequence &tail) const {
	return split_if_once(tail, pred_, head, tail);
}

template <typename Char> inline 
split_if_equal<Char>::split_if_equal(Char value) :
	value_(value)
{
}

template <typename Char> template <typename Sequence> inline bool
split_if_equal<Char>::operator () (Sequence &head, Sequence &tail) const {
	return split_once(tail, value_, head, tail);
}

template <typename Char> inline
split_first_of<Char>::split_first_of(Char const *charset) :
	charset_(charset)
{
}

template <typename Char> template <typename Sequence> inline bool
split_first_of<Char>::operator () (Sequence &head, Sequence &tail) const {
	return split_first_of_once(tail, charset_, head, tail);
}

template <typename Sequence, typename Pred> inline 
token_iterator<Sequence, Pred>::token_iterator(Pred const &pred) :
	pred_(pred), sequence_(), head_(), tail_()
{
}

template <typename Sequence, typename Pred> inline 
token_iterator<Sequence, Pred>::token_iterator(range<typename Sequence::const_iterator> const &range, Pred const &pred) :
	pred_(pred), sequence_(), head_(), tail_(range)
{
	increment();
}

template <typename Sequence, typename Pred> inline void
token_iterator<Sequence, Pred>::increment() {
	pred_(head_, tail_);
	sequence_ = Sequence(head_.begin(), head_.end());
}

template <typename Sequence, typename Pred> inline Sequence const&
token_iterator<Sequence, Pred>::dereference() const {
	return sequence_;
}

template <typename Sequence, typename Pred> inline bool
token_iterator<Sequence, Pred>::equal(token_iterator<Sequence, Pred> const &other) const {
	return (head_ == other.head_) && (tail_ == other.tail_);
}

template <typename Sequence, typename Pred> inline
tokenizer<Sequence, Pred>::tokenizer(Sequence const &sequence, Pred const &pred) :
	pred_(pred), sequence_(sequence)
{
}

template <typename Sequence, typename Pred> inline typename tokenizer<Sequence, Pred>::const_iterator
tokenizer<Sequence, Pred>::end() const {
	return const_iterator(pred_);
}

template <typename Sequence, typename Pred> inline typename tokenizer<Sequence, Pred>::const_iterator
tokenizer<Sequence, Pred>::begin() const {
	range<typename Sequence::const_iterator> range(sequence_.begin(), sequence_.end());
	return const_iterator(range, pred_);
}

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_TOKENIZER_HPP_INCLUDED
