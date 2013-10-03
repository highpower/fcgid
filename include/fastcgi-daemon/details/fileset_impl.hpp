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

#ifndef FASTCGI_DAEMON_DETAILS_FILESET_IMPL_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_FILESET_IMPL_HPP_INCLUDED

#include <memory>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/fileset.hpp"

namespace fcgid { namespace details {

template <typename Iter>
class fileset_impl : public fileset {
	
public:	
	fileset_impl(Iter begin, Iter end);
	virtual void next();
	virtual bool empty() const;
	
	virtual std::string const& name() const;
	virtual file value() const;
	
private:
	fileset_impl(fileset_impl const &);
	fileset_impl& operator = (fileset_impl const &);
	
private:
	Iter i_;
	Iter const end_;
};

template <typename Iter> inline
fileset_impl<Iter>::fileset_impl(Iter begin, Iter end) :
	i_(begin), end_(end)
{
}

template <typename Iter> inline void
fileset_impl<Iter>::next() {
	++i_;
}

template <typename Iter> inline bool
fileset_impl<Iter>::empty() const {
	return end_ == i_;
}

template <typename Iter> inline std::string const&
fileset_impl<Iter>::name() const {
	return i_->first;
}

template <typename Iter> inline file
fileset_impl<Iter>::value() const {
	return file(i_->second);
}

template <typename Iter> inline std::shared_ptr<fileset>
make_fileset(Iter begin, Iter end) {
	return std::shared_ptr<fileset>(new fileset_impl<Iter>(begin, end));
}

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_FILESET_IMPL_HPP_INCLUDED
