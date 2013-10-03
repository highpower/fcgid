#include "acsetup.hpp"
#include "fastcgi-daemon/file.hpp"

#include <algorithm>

#include "fastcgi-daemon/details/file_impl.hpp"

namespace fcgid {

file::file() :
	impl_()
{
}

file::~file() {
}

file::file(std::shared_ptr<file::impl_type> const &impl) :
	impl_(impl)
{
}

file::file(file const &other) :
	impl_(other.impl_)
{
}

file&
file::operator = (file const &other) {
	file copy(other);
	swap(copy);
	return *this;
}

void
file::swap(file &other) throw () {
	using std::swap;
	swap(impl_, other.impl_);
}

std::streamsize
file::size() const {
	return impl_->size();
}

std::string const&
file::name() const {
	return impl_->name();
}

std::string const&
file::content_type() const {
	return impl_->content_type();
}

std::istream&
file::stream() {
	return impl_->stream();
}

void
file::save(char const *name) const {
	impl_->save(name);
}

void
file::save(std::string const &name) const {
	impl_->save(name.c_str());
}

} // namespace
