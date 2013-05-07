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

#ifndef FASTCGI_DAEMON_TESTS_MOCK_IO_HPP_INCLUDED
#define FASTCGI_DAEMON_TESTS_MOCK_IO_HPP_INCLUDED

#include <list>
#include <string>
#include <vector>
#include <fstream>

#include "fastcgi-daemon/config.hpp"

namespace fcgid { namespace tests {

class mock_io {

public:
	typedef char const* descriptor_type;

	mock_io(descriptor_type file);
	char const* const* environ() const;
	std::size_t read(char* buffer, std::size_t size);
	
private:
	mock_io(mock_io const &);
	mock_io& operator = (mock_io const &);
	bool is_empty(std::string const &line) const;
	
private:
	std::ifstream file_;
	std::list<std::string> buf_;
	std::vector<char const*> env_;
};

}} // namespaces

#endif // FASTCGI_DAEMON_TESTS_MOCK_IO_HPP_INCLUDED
