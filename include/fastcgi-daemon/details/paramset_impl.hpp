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

#ifndef FASTCGI_DAEMON_DETAILS_PARAMSET_IMPL_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_PARAMSET_IMPL_HPP_INCLUDED

#include <boost/shared_ptr.hpp>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/paramset.hpp"

namespace fcgid { namespace details {

template <typename Iter>
class paramset_impl : public paramset {

public:
	paramset_impl(Iter begin, Iter end);

	virtual void next();
	virtual bool empty() const;
	
	virtual std::string const& name() const;
	virtual std::string const& value() const;

private:
	paramset_impl(paramset_impl const &);
	paramset_impl& operator = (paramset_impl const &);

private:
	Iter i_;
	Iter const end_;
};

template <typename Iter> inline 
paramset_impl<Iter>::paramset_impl(Iter begin, Iter end) :
	i_(begin), end_(end)
{
}

template <typename Iter> inline void
paramset_impl<Iter>::next() {
	++i_;
}

template <typename Iter> inline bool
paramset_impl<Iter>::empty() const {
	return i_ == end_;
}

template <typename Iter> inline std::string const&
paramset_impl<Iter>::name() const {
	return i_->first;
}

template <typename Iter> inline std::string const&
paramset_impl<Iter>::value() const {
	return i_->second;
}

template <typename Iter> boost::shared_ptr<paramset>
make_paramset(Iter begin, Iter end) {
	return boost::shared_ptr<paramset>(new paramset_impl<Iter>(begin, end));
}

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_PARAMSET_IMPL_HPP_INCLUDED
