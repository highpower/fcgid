#include "acsetup.hpp"
#include "fastcgi-daemon/http_status.hpp"

namespace fcgid {

http_status const http_status::bad(0);

http_status const http_status::ok(200);
http_status const http_status::created(201);
http_status const http_status::accepted(202);
http_status const http_status::non_authoritative(203);
http_status const http_status::no_content(204);

http_status const http_status::moved_permanently(301);
http_status const http_status::moved_temporarily(302);
http_status const http_status::see_other(303);
http_status const http_status::not_modified(304);

http_status const http_status::bad_request(400);
http_status const http_status::unauthorized(401);
http_status const http_status::payment_required(402);
http_status const http_status::forbidden(403);
http_status const http_status::not_found(404);
http_status const http_status::method_not_allowed(405);
http_status const http_status::entity_too_large(413);
http_status const http_status::range_not_satisfiable(416);

http_status const http_status::internal_error(500);
http_status const http_status::not_implemented(501);
http_status const http_status::bad_gateway(502);
http_status const http_status::service_unavailable(503);
http_status const http_status::gateway_timeout(504);
http_status const http_status::version_not_supported(505);

http_status::http_status(long code) :
	code_(code)
{
}

void
http_status::swap(http_status &other) throw () {
	using std::swap;
	swap(code_, other.code_);
}

char const*
http_status::message() const {

	switch (code_) {

		case 200: return "200 OK";
		case 201: return "201 Created";
		case 202: return "202 Accepted";
		case 203: return "203 Non-Authoritative Information";
		case 204: return "204 No Content";

		case 301: return "301 Moved Permanently";
		case 302: return "302 Moved Temporarily";
		case 303: return "303 See Other";
		case 304: return "304 Not Modified";

		case 400: return "400 Bad Request";
		case 401: return "401 Unauthorized";
		case 402: return "402 PaymentRequired";
		case 403: return "403 Forbidden";
		case 404: return "404 Not Found";
		case 405: return "405 Method Not Allowed";
		case 416: return "416 Requested Range Not Satisfiable";

		case 500: return "500 Internal Server Error";
		case 501: return "501 Not Implemented";
		case 502: return "502 Bad Gateway";
		case 503: return "503 Service Unavailable";
		case 504: return "504 Gateway Timeout";
		case 505: return "505 HTTP Version Not Supported";
	}
	return "Unknown Status";
}

bool
http_status::is_success() const {
	return (code_ >= 200) && (code_ < 300);
}

bool
http_status::is_redirect() const {
	return (code_ >= 300) && (code_ < 400);
}

bool
http_status::is_client_error() const {
	return (code_ >= 400) && (code_ < 500);
}

bool
http_status::is_server_error() const {
	return (code_ >= 500) && (code_ < 600);
}

} // namespace
