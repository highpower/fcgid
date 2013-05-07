#include "acsetup.hpp"
#include "fastcgi-daemon/details/generic_response.hpp"

namespace fcgid { namespace details {

range<char const*> const
generic_response_base::status_token = make_range("Status");

range<char const*> const
generic_response_base::expire_time_token = make_range("Expires");

range<char const*> const
generic_response_base::location_token = make_range("Location");

range<char const*> const
generic_response_base::set_cookie_token = make_range("Set-Cookie");

range<char const*> const
header_constants::p3p_header_token = make_range("P3P");


}} // namespaces
