#include "acsetup.hpp"
#include "mock_handler.hpp"

namespace fcgid { namespace tests {

mock_request::mock_request(std::string const &pi) :
	path_info_(pi)
{
}

std::string const&
mock_request::path_info() const {
	return path_info_;
}

mock_response::mock_response() :
	content_()
{
}

std::string const&
mock_response::content() const {
	return content_;
}

void
mock_response::content(std::string const &cont) {
	content_ = cont;
}

mock_context::mock_context(const std::string &pi) :
	request_(pi), response_()
{
}

mock_response&
mock_context::response() {
	return response_;
}
mock_request const&
mock_context::request() const {
	return request_;
}

mock_handler::mock_handler() :
	name_()
{
}

mock_handler::mock_handler(std::string const &name) :
	name_(name)
{
}

void
mock_handler::handle(boost::shared_ptr<mock_context> const &ctx, logger &log) const {
	mock_response &resp = ctx->response();
	resp.content(name_);
}

mock_handler::operator mock_handler::bool_convertible const* () const {
	return reinterpret_cast<bool_convertible const*>(!name_.empty());
}

}} // namespaces
