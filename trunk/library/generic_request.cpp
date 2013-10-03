#include "acsetup.hpp"
#include "fastcgi-daemon/details/generic_request.hpp"

namespace fcgid { namespace details {

range<char const*> const
generic_request_base::content_length_token = make_range("HTTP_CONTENT_LENGTH");

range<char const*> const
generic_request_base::request_method_token = make_range("REQUEST_METHOD");

}} // namespaces
