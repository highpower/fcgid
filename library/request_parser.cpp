#include "acsetup.hpp"
#include "fastcgi-daemon/details/request_parser.hpp"

namespace fcgid { namespace details {

range<char const*> const
request_parser_base::nn_token = make_range("\n\n");

range<char const*> const
request_parser_base::rnrn_token = make_range("\r\n\r\n");

range<char const*> const
request_parser_base::cookie_token = make_range("HTTP_COOKIE");

range<char const*> const
request_parser_base::boundary_token = make_range("boundary");

range<char const*> const
request_parser_base::query_string_token = make_range("QUERY_STRING");

range<char const*> const
request_parser_base::multipart_ending_token = make_range("--");

range<char const*> const
request_parser_base::multipart_form_data_token = make_range("multipart/form-data");

range<char const*> const
multipart_header_parser_base::name_token = make_range("name");

range<char const*> const
multipart_header_parser_base::file_name_token = make_range("filename");

range<char const*> const
multipart_header_parser_base::form_data_token = make_range("form-data");

range<char const*> const
multipart_header_parser_base::content_type_token = make_range("Content-Type");

range<char const*> const
multipart_header_parser_base::content_disposition_token = make_range("Content-Disposition");

}} // namespaces
