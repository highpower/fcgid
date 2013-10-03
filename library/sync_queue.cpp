#include "acsetup.hpp"
#include "fastcgi-daemon/details/sync_queue.hpp"
#include "fastcgi-daemon/details/runtime_error.hpp"

namespace fcgid { namespace details {

char const*
bounds_reached::what() const throw () {
	return "bound reached";
}

bool
null_bounds_checker::reached(std::size_t size) const {
	(void) size;
	return false;
}

dynamic_bounds_checker::dynamic_bounds_checker() :
	size_(0)
{
}

void
dynamic_bounds_checker::bounds(std::size_t size) {
	size_ = size;
}

bool
dynamic_bounds_checker::reached(std::size_t size) const {
	if (0 != size_) {
		return size_ == size;
	}
	throw runtime_error("thread queue was not initialized properly");
}

}} // namespaces
