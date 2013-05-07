#include "acsetup.hpp"
#include "fastcgi-daemon/details/post_buffer.hpp"

#include <new>
#include <cstdlib>
#include <iostream>
#include <algorithm>

namespace fcgid { namespace details {

post_buffer::post_buffer() :
	begin_(0), end_(0), buffer_()
{
}

void
post_buffer::reserve(std::size_t size) {
	buffer_.reset(static_cast<char*>(malloc(size)));
	if (buffer_) {
		begin_ = buffer_.get();
		end_ = begin_ + size;
	}
	else throw std::bad_alloc();
}

bool
post_buffer::empty() const {
	return (begin_ == end_);
}

post_buffer::size_type
post_buffer::size() const {
	return static_cast<size_type>(std::distance(begin_, end_));
}

std::ostream&
post_buffer::write(std::ostream &out) {
	out.write(begin_, std::distance(begin_, end_));
	return out;
}

void
malloc_memory_traits::destroy(char *value) {
	free(value);
}

char*
malloc_memory_traits::default_value() {
	return static_cast<char*>(0);
}

}} // namespaces
