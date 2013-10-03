#include "acsetup.hpp"
#include "fastcgi-daemon/cookie.hpp"
#include "fastcgi-daemon/details/cookie_impl.hpp"

namespace fcgid {

cookie::cookie(std::string const &name, std::string const &value) :
	impl_(new impl_type(name, value))
{
}

cookie::~cookie() {
}

cookie::cookie(cookie const &other) :
	impl_(other.impl_)
{
}

cookie&
cookie::operator = (cookie const &other) {
	cookie copy(other);
	swap(copy);
	return *this;
}

void
cookie::swap(cookie &other) throw () {
	impl_.swap(other.impl_);
}

std::string const&
cookie::name() const {
	return impl_->name();
}

std::string const&
cookie::value() const {
	return impl_->value();
}

bool
cookie::secure() const {
	return impl_->secure();
}

void
cookie::secure(bool value) {
	impl_->secure(value);
}

bool
cookie::http_only() const {
	return impl_->http_only();
}

void
cookie::http_only(bool value) {
	impl_->http_only(value);
}

std::string const&
cookie::path() const {
	return impl_->path();
}

void
cookie::path(std::string const &value) {
	impl_->path(value);
}

std::string const&
cookie::domain() const {
	return impl_->domain();
}

void
cookie::domain(std::string const &value) {
	impl_->domain(value);
}

http_date const&
cookie::expire_time() const {
	return impl_->expire_time();
}

void
cookie::expire_time(http_date const& value) {
	impl_->expire_time(value);
}

void
cookie::expire_time(std::string const &value) {
	impl_->expire_time(value);
}

std::string
cookie::str() const {
	return impl_->str();
}

std::shared_ptr<cookie::impl_type> const&
cookie::impl() const {
	return impl_;
}

} // namespace
