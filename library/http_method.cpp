#include "acsetup.hpp"
#include "fastcgi-daemon/http_method.hpp"

#include <algorithm>

#include "fastcgi-daemon/details/range.hpp"
#include "fastcgi-daemon/details/functors.hpp"

namespace fcgid {

static details::range<char const*> const
http_method_get_token = details::make_range("get");

static details::range<char const*> const
http_method_head_token = details::make_range("head");

static details::range<char const*> const
http_method_post_token = details::make_range("post");

http_method const
http_method::get(1);

http_method const
http_method::head(2);

http_method const
http_method::post(4);

http_method const
http_method::unknown(0);

http_method_mask const
http_method_mask::all(http_method::get | http_method::head | http_method::post);

http_method_mask const
http_method_mask::none(http_method::unknown);

http_method_mask const
http_method_mask::get_only(http_method::get | http_method::head);

http_method_mask const
http_method_mask::post_only(http_method::post | http_method::head);

http_method_mask::http_method_mask() :
	mask_(0)
{
}

http_method_mask::http_method_mask(http_method const &mth) :
	mask_(mth.code())
{
}

void
http_method_mask::swap(http_method_mask &other) throw () {
	using std::swap;
	swap(mask_, other.mask_);
}

void
http_method_mask::extend(http_method_mask const &m) {
	mask_ |= m.mask_;
}

bool
http_method_mask::accepts(http_method const &m) const {
	return (mask_ & m.code());
}

http_method::http_method(long code) :
	code_(code)
{
}

void
http_method::swap(http_method &other) throw () {
	using std::swap;
	swap(code_, other.code_);
}

char const*
http_method::name() const {
	if (get.code_ == code_) {
		return http_method_get_token.begin();
	}
	else if (head.code_ == code_) {
		return http_method_head_token.begin();
	}
	else if (post.code_ == code_) {
		return http_method_post_token.begin();
	}
	return "unknown method";
}

http_method
http_method::parse(char const *str) {
	details::range<char const*> method = details::make_range(str);
	if (is_ci_equal(http_method_get_token, method)) {
		return http_method::get;
	}
	else if (is_ci_equal(http_method_head_token, method)) {
		return http_method::head;
	}
	else if (is_ci_equal(http_method_post_token, method)) {
		return http_method::post;
	}
	return http_method::unknown;
}

http_method_mask
operator | (http_method const &method, http_method const &other) {
	return http_method_mask(method) | http_method_mask(other);
}

http_method_mask
operator | (http_method_mask const &target, http_method_mask const &other) {
	http_method_mask temp(target);
	temp.extend(other);
	return temp;
}

} // namespaces
