// fastcgi-daemon is an advanced wrapper around libfcgi library written in c++
// Copyright (C) 2013 Rambler <highpower@rambler.ru>

// fastcgi-daemon is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef FASTCGI_DAEMON_DETAILS_REQUEST_PARSER_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_REQUEST_PARSER_HPP_INCLUDED

#include <string>
#include <utility>
#include <boost/shared_ptr.hpp>

#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/http_error.hpp"
#include "fastcgi-daemon/http_method.hpp"
#include "fastcgi-daemon/http_status.hpp"

#include "fastcgi-daemon/details/range.hpp"
#include "fastcgi-daemon/details/strings.hpp"
#include "fastcgi-daemon/details/functors.hpp"
#include "fastcgi-daemon/details/iter_file.hpp"
#include "fastcgi-daemon/details/urlencode_impl.hpp"
#include "fastcgi-daemon/details/line_end_filter.hpp"

namespace fcgid { namespace details {

struct request_parser_base {
	static range<char const*> const nn_token;
	static range<char const*> const rnrn_token;
	static range<char const*> const cookie_token;
	static range<char const*> const boundary_token;
	static range<char const*> const query_string_token;
	static range<char const*> const multipart_ending_token;
	static range<char const*> const multipart_form_data_token;
};

template <typename Request>
class request_parser : private request_parser_base {

public:
	typedef Request request_type;
	typedef typename request_type::io_type io_type;
	typedef typename request_type::buffer_type buffer_type;
	typedef typename request_type::string_type string_type;
	
	typedef typename string_type::allocator_type allocator_type;
	typedef typename buffer_type::const_iterator buffer_iterator_type;
	typedef file_impl<allocator_type> file_type;
	
	request_parser(request_type &req);
	void parse(std::size_t threshold);

private:
	void parse_var(range<char const*> const &r);
	void parse_cookie(range<char const*> const &r);
	void parse_cookies(range<char const*> const &r);
	
	void parse_post(std::size_t threshold);
	void parse_plain_post(buffer_type const &buffer);
	void parse_multipart_post(buffer_type const &buffer, string_type const &bound);
	
	void parse_part(range<buffer_iterator_type> const &r);
	void process_part(range<buffer_iterator_type> const &header, range<buffer_iterator_type> const &content);
	
	string_type boundary(range<typename string_type::const_iterator> const &r);
	range<typename string_type::const_iterator> strip_boundary_quotes(range<typename string_type::const_iterator> const &r);
	
	template <typename Iter> void parse_arg(range<Iter> const &r);
	template <typename Iter> void parse_query_string(Iter begin, Iter end);

	using request_parser_base::nn_token;
	using request_parser_base::rnrn_token;
	using request_parser_base::cookie_token;
	using request_parser_base::boundary_token;
	using request_parser_base::query_string_token;
	using request_parser_base::multipart_ending_token;
	using request_parser_base::multipart_form_data_token;
	
private:
	request_type &request_;
};

struct multipart_header_parser_base {
	static range<char const*> const name_token;
	static range<char const*> const file_name_token;
	static range<char const*> const form_data_token;
	static range<char const*> const content_type_token;
	static range<char const*> const content_disposition_token;
};

template <typename Iter>
class multipart_header_parser : private multipart_header_parser_base {

public:
	typedef Iter iterator;
	typedef typename std::iterator_traits<Iter>::value_type value_type;
	typedef typename line_end_filter<iterator>::type filtered_iterator_type;
	typedef range<filtered_iterator_type> filtered_range_type;

	multipart_header_parser(Iter begin, Iter end);

	bool is_file() const;
	range<Iter> const& name() const;
	range<Iter> const& file_name() const;
	range<Iter> const& content_type() const;

private:
	multipart_header_parser(multipart_header_parser const &);
	multipart_header_parser& operator = (multipart_header_parser const &);

	void parse(filtered_range_type const &filtered);
	void parse_line(filtered_range_type const &filtered);
	void parse_content_disposition(filtered_range_type const &filtered);
	void parse_disposition_attribute(filtered_range_type const &filtered);

	using multipart_header_parser_base::name_token;
	using multipart_header_parser_base::file_name_token;
	using multipart_header_parser_base::form_data_token;
	using multipart_header_parser_base::content_type_token;
	using multipart_header_parser_base::content_disposition_token;

private:
	is_space<value_type> space_checker_;
	is_line_end<value_type> line_end_checker_;
	range<Iter> name_, type_, file_name_;
};

template <typename Sequence> inline std::pair<Sequence, Sequence>
split_key_value(Sequence const &seq) {

	Sequence head, tail;
	split_once(seq, '=', head, tail);
	return std::make_pair(trim(head), trim(tail));
}

template <typename Request> inline
request_parser<Request>::request_parser(typename request_parser<Request>::request_type &req) :
	request_(req)
{
}

template <typename Request> inline void
request_parser<Request>::parse(std::size_t threshold) {
	io_type &io = request_.io();
	char const* const* env = io.environ();
	for (std::size_t i = 0; static_cast<char const*>(0) != env[i]; ++i) {
		parse_var(make_range(env[i]));
	}
	http_method method = request_.method();
	if (http_method::post == method) {
		parse_post(threshold);
	}
	else if (http_method::unknown == method) {
		throw http_error(http_status::method_not_allowed);
	}
}

template <typename Request> inline void
request_parser<Request>::parse_var(range<char const*> const &r) {

	typedef range<char const*> range_type;
	std::pair<range_type, range_type> p = split_key_value(r);
	
	range_type const &head = p.first;
	range_type const &tail = p.second;
	
	string_type const &pattern = request_.pattern();
	request_.add_var(
		string_type(head.begin(), head.end(), pattern.get_allocator()), 
		string_type(tail.begin(), tail.end(), pattern.get_allocator()));
	
	if (cookie_token == head) {
		parse_cookies(tail);
	}
	else if (query_string_token == head) {
		parse_query_string(tail.begin(), tail.end());
	}
}

template <typename Request> inline void
request_parser<Request>::parse_cookie(range<char const*> const &r) {
	typedef range<char const*> range_type;
	string_type const &pattern = request_.pattern();
	std::pair<range_type, range_type> p = split_key_value(r);
	request_.add_cookie(urldecode(p.first, pattern), urldecode(p.second, pattern));
}

template <typename Request> inline void
request_parser<Request>::parse_cookies(range<char const*> const &r) {
	range<char const*> head, tail = r;
	while (!tail.empty()) {
		split_once(tail, ';', head, tail);
		parse_cookie(head);
		trim_left(tail);
	}
}

template <typename Request> inline void
request_parser<Request>::parse_post(std::size_t threshold) {

	(void) threshold;
	
	buffer_type buffer = request_.read_post();
	string_type const &type = request_.content_type();
	
	range<typename string_type::const_iterator> type_range(type.begin(), type.end());
	type_range = trim(type_range);
	
	if (starts_with(type_range, multipart_form_data_token)) {
		range<typename string_type::const_iterator> head, tail;
		split_once(type_range, ';', head, tail);
		parse_multipart_post(buffer, boundary(trim(tail)));
	}
	else {
		parse_plain_post(buffer);
	}
}

template <typename Request> inline void
request_parser<Request>::parse_plain_post(typename request_parser<Request>::buffer_type const &buf) {
	parse_query_string(buf.begin(), buf.end());
}

template <typename Request> inline void
request_parser<Request>::parse_multipart_post(typename request_parser<Request>::buffer_type const &buf, typename request_parser<Request>::string_type const &bound) {

	bool ending_found = false;
	is_line_end<typename buffer_type::value_type> checker;
	range<buffer_iterator_type> head, tail(buf.begin(), buf.end());
	while (!tail.empty()) {
		split_once(tail, bound, head, tail);
		head = trim_if(head, checker);
		if (multipart_ending_token == head) {
			ending_found = true;
		}
		else if (!head.empty()) {
			parse_part(head);
		}
	}
	if (!ending_found) {
		throw http_error(http_status::bad_request);
	}
}

template <typename Request> inline void
request_parser<Request>::parse_part(range<typename request_parser<Request>::buffer_iterator_type> const &r) {

	is_line_end<typename buffer_type::value_type> checker;
	buffer_iterator_type const begin = r.begin(), end = r.end();
	buffer_iterator_type line_end, new_line = begin;

	while (true) {
		line_end = next_matched(new_line, end, checker);
 		new_line = next_not_matched(line_end, end, checker);
		range<buffer_iterator_type> token(line_end, new_line);
		if (token.empty()) {
			throw http_error(http_status::bad_request);
		}
		else if (rnrn_token == token || nn_token == token) {
			break;
		}
	}
	range<buffer_iterator_type> head = make_range(begin, line_end);
	range<buffer_iterator_type> content = make_range(new_line, end);
	process_part(head, content);
}

template <typename Request> inline void
request_parser<Request>::process_part(range<typename request_parser<Request>::buffer_iterator_type> const &header, details::range<typename request_parser<Request>::buffer_iterator_type> const &content) {
	
	typedef range<buffer_iterator_type> range_type;
	string_type const &pattern = request_.pattern();
	multipart_header_parser<typename range_type::const_iterator> parser(header.begin(), header.end());

	range_type const &name_range = parser.name();
	string_type name = string_type(name_range.begin(), name_range.end(), pattern.get_allocator());
	
	if (parser.is_file()) {
		range_type const &file_name_range = parser.file_name();
		string_type file_name(file_name_range.begin(), file_name_range.end(), pattern.get_allocator());
		
		range_type const &content_type_range = parser.content_type();
		string_type content_type(content_type_range.begin(), content_type_range.end(), pattern.get_allocator());
		boost::shared_ptr<file_type> f(new iter_file<buffer_iterator_type, allocator_type>(file_name, content_type, content));
		request_.add_file(name, f);
	}
	else {
		request_.add_arg(name, string_type(content.begin(), content.end(), pattern.get_allocator()));
	}
}

template <typename Request> inline typename request_parser<Request>::string_type
request_parser<Request>::boundary(range<typename request_parser<Request>::string_type::const_iterator> const &r) {

	typedef range<typename string_type::const_iterator> range_type;
	if (!starts_with(r, boundary_token)) {
		throw http_error(http_status::bad_request);
	}
	
	string_type result(request_.pattern());
	std::pair<range_type, range_type> p = split_key_value(r);
	range_type boundary = strip_boundary_quotes(trim(p.second));
	
	result.append("--");
	result.append(boundary.begin(), boundary.end());
	return result;
}

template <typename Request> inline range<typename request_parser<Request>::string_type::const_iterator>
request_parser<Request>::strip_boundary_quotes(range<typename request_parser<Request>::string_type::const_iterator> const &r) {
	if ((r.size() < 2) || '"' != *(r.begin())) {
		return r;
	}
	if ('"' == *(r.rbegin())) {
		return trim_chars(r, '"');
	}
	throw http_error(http_status::bad_request);
}

template <typename Request> template <typename Iter> inline void
request_parser<Request>::parse_arg(range<Iter> const &r) {
	typedef range<Iter> range_type;
	string_type const &pattern = request_.pattern();
	std::pair<range_type, range_type> p = split_key_value(r);
	request_.add_arg(urldecode(p.first, pattern), urldecode(p.second, pattern));
}

template <typename Request> template <typename Iter> inline void
request_parser<Request>::parse_query_string(Iter begin, Iter end) {
	range<Iter> head, tail(begin, end);
	while (!tail.empty()) {
		split_first_of_once(tail, "&;", head, tail);
		parse_arg(head);
	}
}

template <typename Iter> inline
multipart_header_parser<Iter>::multipart_header_parser(Iter begin, Iter end) :
	space_checker_(), line_end_checker_(), name_(), type_(), file_name_()
{
	parse(make_line_end_filtered(begin, end));
}

template <typename Iter> inline bool
multipart_header_parser<Iter>::is_file() const {
	return !file_name_.empty();
}

template <typename Iter> inline range<Iter> const&
multipart_header_parser<Iter>::name() const {
	return name_;
}

template <typename Iter> inline range<Iter> const&
multipart_header_parser<Iter>::file_name() const {
	return file_name_;
}

template <typename Iter> inline range<Iter> const&
multipart_header_parser<Iter>::content_type() const {
	return type_;
}

template <typename Iter> inline void
multipart_header_parser<Iter>::parse(typename multipart_header_parser<Iter>::filtered_range_type const &filtered) {
	
	filtered_range_type r = filtered;
	is_line_end<typename filtered_range_type::value_type> checker;
	while (!r.empty()) {
		typename filtered_range_type::iterator begin = r.begin(), end = r.end();
		typename filtered_range_type::iterator line_end = next_matched(begin, end, checker);
		typename filtered_range_type::iterator new_line = next_not_matched(line_end, end, checker);
		parse_line(make_range(begin, line_end));
		r = make_range(new_line, end);
	}
}

template <typename Iter> inline void
multipart_header_parser<Iter>::parse_line(typename multipart_header_parser<Iter>::filtered_range_type const &filtered) {
	
	filtered_range_type head, tail;
	split_once(filtered, ':', head, tail);
	tail = trim(tail);
	if (is_ci_equal(content_type_token, head)) {
		type_ = make_line_end_unfiltered(tail.begin(), tail.end());
	}
	else if (is_ci_equal(content_disposition_token, head)) {
		parse_content_disposition(tail);
	}
}

template <typename Iter> inline void
multipart_header_parser<Iter>::parse_content_disposition(typename multipart_header_parser<Iter>::filtered_range_type const &filtered) {

	if (!starts_with(filtered, form_data_token)) {
		throw http_error(http_status::bad_request);
	}
	filtered_range_type r = filtered, part;
	while (!r.empty()) {
		split_once(r, ';', part, r);
		parse_disposition_attribute(trim(part));
	}
}

template <typename Iter> inline void
multipart_header_parser<Iter>::parse_disposition_attribute(typename multipart_header_parser<Iter>::filtered_range_type const &filtered) {

	if (form_data_token != filtered) {
		std::pair<filtered_range_type, filtered_range_type> p = split_key_value(filtered);
		filtered_range_type value = trim_chars(p.second, '"');
		if (is_ci_equal(name_token, p.first)) {
			name_ = make_line_end_unfiltered(value.begin(), value.end());
		}
		else if (is_ci_equal(file_name_token, p.first)) {
			file_name_ = make_line_end_unfiltered(value.begin(), value.end());
		}
	}
}

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_REQUEST_PARSER_HPP_INCLUDED
