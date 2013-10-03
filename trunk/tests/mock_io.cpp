#include "acsetup.hpp"
#include "mock_io.hpp"

#include <cassert>

#include "fastcgi-daemon/details/strings.hpp"

namespace fcgid { namespace tests {

mock_io::mock_io(mock_io::descriptor_type file) :
	file_(), buf_(), env_()
{
	file_.exceptions(std::ios::badbit | std::ios::failbit);
	file_.open(file);

	std::string line;
	while (std::getline(file_, line)) {
		if (!is_empty(line)) {
			buf_.push_back(line);
		}
		else break;
	}
	env_.reserve(buf_.size());
	for (std::list<std::string>::iterator i = buf_.begin(), end = buf_.end(); i != end; ++i) {
		env_.push_back(i->c_str());
	}
	env_.push_back(static_cast<char const*>(0));
}

char const* const* 
mock_io::environ() const {
	assert(!env_.empty());
	return &env_[0];
}

std::size_t
mock_io::read(char *buffer, std::size_t size) {
	file_.read(buffer, size);
	return file_.gcount();
}

bool
mock_io::is_empty(std::string const &line) const {
	details::is_line_end<char> checker;
	if (line.empty()) {
		return true;
	}
	else if (checker(line[0])) {
		return (line.end() == details::next_not_matched(line.begin(), line.end(), checker));
	}
	return false;
}

}} // namespaces
