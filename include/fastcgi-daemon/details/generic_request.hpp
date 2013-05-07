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

#ifndef FASTCGI_DAEMON_DETAILS_GENERIC_REQUEST_HPP_INCLUDED
#define FASTCGI_DAEMON_DETAILS_GENERIC_REQUEST_HPP_INCLUDED

#include <map>
#include <string>
#include <fstream>
#include <functional>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits.hpp>

#include "fastcgi-daemon/file.hpp"
#include "fastcgi-daemon/config.hpp"
#include "fastcgi-daemon/http_method.hpp"
#include "fastcgi-daemon/details/resource.hpp"
#include "fastcgi-daemon/details/post_buffer.hpp"
#include "fastcgi-daemon/details/runtime_error.hpp"
#include "fastcgi-daemon/details/request_parser.hpp"

namespace fcgid { namespace details {

struct generic_request_base {
	static range<char const*> const content_length_token;
	static range<char const*> const request_method_token;
};

template <typename IO, typename Alloc>
class generic_request : public generic_request_base {

public:
	typedef IO io_type;
	typedef Alloc allocator_type;
	typedef range<char const*> buffer_type;
	typedef generic_request<IO, Alloc> type;
	
	typedef typename Alloc::template rebind<char>::other char_allocator_type;
	typedef std::basic_string<char, std::char_traits<char>, char_allocator_type> string_type;
	
	typedef std::pair<string_type const, string_type> param_type;
	typedef typename Alloc::template rebind<param_type>::other param_allocator_type;
	
	typedef std::map<string_type, string_type, std::less<string_type>, param_allocator_type> string_map_type;
	typedef std::multimap<string_type, string_type, std::less<string_type>, param_allocator_type> string_multimap_type;
	
	typedef file_impl<allocator_type> file_type;
	typedef boost::shared_ptr<file_type> file_ptr_type;

	typedef std::pair<string_type const, file_ptr_type> file_param_type;
	typedef typename Alloc::template rebind<file_param_type>::other file_param_allocator_type;
	typedef std::map<string_type, file_ptr_type, std::less<string_type>, file_param_allocator_type> file_map_type;

	generic_request(io_type &io, Alloc const &a = Alloc());
	virtual ~generic_request();

	io_type& io();
	bool secure() const;
	http_method method() const;

	std::size_t content_length() const;
	string_type const& path_info() const;
	string_type const& content_type() const;

	string_map_type const& vars() const;
	bool has_var(string_type const &name) const;
	string_type const& var(string_type const &name) const;
	void add_var(string_type const &name, string_type const &value);

	string_map_type const& cookies() const;
	bool has_cookie(string_type const &name) const;
	string_type const& cookie(string_type const &name) const;
	void add_cookie(string_type const &name, string_type const &value);

	string_multimap_type const& args() const;
	bool has_arg(string_type const &name) const;
	string_type const& arg(string_type const &name) const;
	void add_arg(typename generic_request<IO, Alloc>::string_type const &name, string_type const &value);

	file_map_type const& files() const;
	bool has_file(string_type const &name) const;
	file_ptr_type const& get_file(string_type const &name) const;
	void add_file(string_type const &name, file_ptr_type const &f);

	bool has_header(string_type const &name) const;
	string_type const& header(string_type const &name) const;
	
	void parse();
	buffer_type read_post();

	void store(char const *name);
	string_type const& pattern() const;

private:
	generic_request(generic_request const &);
	generic_request& operator = (generic_request const &);
	
	using generic_request_base::content_length_token;
	using generic_request_base::request_method_token;
	
	template <typename Map> bool map_has(Map const &m, string_type const &key) const;
	template <typename Map> string_type const& map_get(Map const &m, string_type const &key) const;

private:	
	io_type &io_;
	http_method method_;
	post_buffer buffer_;
	string_multimap_type args_;
	string_map_type vars_, cookies_;
	file_map_type files_;
	string_type const empty_string_;
	std::size_t content_length_;
};

template <typename IO>
class io_reader : public std::binary_function<char*, std::size_t, std::size_t> {

public:
	typedef IO io_type;
	io_reader(io_type &io);
	std::size_t operator () (char*, std::size_t size);

private:
	io_type &io_;
};

template <typename String, typename Sequence> inline String
make_var_name_impl(Sequence const &sequence, String &result) {
	
	typedef typename boost::remove_cv<typename Sequence::value_type>::type char_type;
	if (sequence.empty()) {
		throw runtime_error("%s", "sequence can not be empty");
	}
	else {
		result.append("HTTP_");
		Sequence trimmed = trim(sequence);
		result.reserve(result.size() + trimmed.size());
		for (typename Sequence::const_iterator i = trimmed.begin(), end = trimmed.end(); i != end; ++i) {
			if (char_traits<char_type>::is_alpha_numeric(*i)) {
				result.push_back(char_traits<char_type>::to_upper(*i));
			}
			else if (static_cast<char_type>('-') == (*i)) {
				result.push_back(static_cast<char_type>('_'));
			}
		}
		return result;
	}
}

template <typename String, typename Sequence> inline String
make_var_name(Sequence const &sequence) {
	String result;
	return make_var_name_impl(sequence, result);
}

template <typename String, typename Sequence> inline String
make_var_name(Sequence const &sequence, String const &pattern) {
	String result(pattern);
	return make_var_name_impl(sequence, result);
}

template <typename String> inline String
make_var_name(String const &header) {
	String result;
	return make_var_name_impl(header, result); 
}

inline std::string
make_var_name(char const *value) {
	return make_var_name<std::string>(make_range(value));
}

template <typename IO, typename Alloc> inline 
generic_request<IO, Alloc>::generic_request(typename generic_request<IO, Alloc>::io_type &io, typename generic_request<IO, Alloc>::allocator_type const &alloc) :
	io_(io), method_(http_method::unknown), buffer_(), 
	args_(std::less<string_type>(), param_allocator_type(alloc)), 
	vars_(std::less<string_type>(), param_allocator_type(alloc)), 
	cookies_(std::less<string_type>(), param_allocator_type(alloc)), 
	files_(std::less<string_type>(), file_param_allocator_type(alloc)), 
	empty_string_(char_allocator_type(alloc)), content_length_(0)
{
}

template <typename IO, typename Alloc> inline 
generic_request<IO, Alloc>::~generic_request() {
}

template <typename IO, typename Alloc> inline typename generic_request<IO, Alloc>::io_type&
generic_request<IO, Alloc>::io() {
	return io_;
}

template <typename IO, typename Alloc> inline bool
generic_request<IO, Alloc>::secure() const {
	return has_var("HTTPS");
}

template <typename IO, typename Alloc> inline http_method
generic_request<IO, Alloc>::method() const {
	return method_;
}

template <typename IO, typename Alloc> inline std::size_t
generic_request<IO, Alloc>::content_length() const {
	return content_length_;
}

template <typename IO, typename Alloc> inline typename generic_request<IO, Alloc>::string_type const&
generic_request<IO, Alloc>::path_info() const {
	string_type name("PATH_INFO", pattern().get_allocator());
	return var(name);
}

template <typename IO, typename Alloc> inline typename generic_request<IO, Alloc>::string_type const&
generic_request<IO, Alloc>::content_type() const {
	string_type name("CONTENT_TYPE", pattern().get_allocator());
	return var(name);
}

template <typename IO, typename Alloc> inline typename generic_request<IO, Alloc>::string_map_type const&
generic_request<IO, Alloc>::vars() const {
	return vars_;
}

template <typename IO, typename Alloc> inline bool
generic_request<IO, Alloc>::has_var(typename generic_request<IO, Alloc>::string_type const &name) const {
	return map_has(vars_, name);
}

template <typename IO, typename Alloc> inline typename generic_request<IO, Alloc>::string_type const&
generic_request<IO, Alloc>::var(typename generic_request<IO, Alloc>::string_type const &name) const {
	return map_get(vars_, name);
}

template <typename IO, typename Alloc> inline void
generic_request<IO, Alloc>::add_var(typename generic_request<IO, Alloc>::string_type const &name, typename generic_request<IO, Alloc>::string_type const &value) {
	range<typename string_type::const_iterator> name_range = make_range(name.begin(), name.end());
	if (request_method_token == name_range) {
		method_ = http_method::parse(value.c_str());
	}
	else if (content_length_token == name_range) {
		char *err = 0;
		unsigned long long result = strtoull(value.c_str(), &err, 10);
		if (static_cast<char>(0) != *err) {
			throw http_error(http_status::bad_request);
		}
		content_length_ = static_cast<std::size_t>(result);
	}
	vars_[name] = value;
}

template <typename IO, typename Alloc> inline typename generic_request<IO, Alloc>::string_map_type const&
generic_request<IO, Alloc>::cookies() const {
	return cookies_;
}

template <typename IO, typename Alloc> inline bool
generic_request<IO, Alloc>::has_cookie(typename generic_request<IO, Alloc>::string_type const &name) const {
	return map_has(cookies_, name);
}

template <typename IO, typename Alloc> inline typename generic_request<IO, Alloc>::string_type const&
generic_request<IO, Alloc>::cookie(typename generic_request<IO, Alloc>::string_type const &name) const {
	return map_get(cookies_, name);
}

template <typename IO, typename Alloc> inline void
generic_request<IO, Alloc>::add_cookie(typename generic_request<IO, Alloc>::string_type const &name, typename generic_request<IO, Alloc>::string_type const &value) {
	cookies_[name] = value;
}

template <typename IO, typename Alloc> inline typename generic_request<IO, Alloc>::string_multimap_type const&
generic_request<IO, Alloc>::args() const {
	return args_;
}

template <typename IO, typename Alloc> inline bool
generic_request<IO, Alloc>::has_arg(typename generic_request<IO, Alloc>::string_type const &name) const {
	return map_has(args_, name);
}

template <typename IO, typename Alloc> inline typename generic_request<IO, Alloc>::string_type const&
generic_request<IO, Alloc>::arg(typename generic_request<IO, Alloc>::string_type const &name) const {
	return map_get(args_, name);
}

template <typename IO, typename Alloc> inline void
generic_request<IO, Alloc>::add_arg(typename generic_request<IO, Alloc>::string_type const &name, typename generic_request<IO, Alloc>::string_type const &value) {
	args_.insert(std::make_pair(name, value));
}

template <typename IO, typename Alloc> inline typename generic_request<IO, Alloc>::file_map_type const&
generic_request<IO, Alloc>::files() const {
	return files_;
}

template <typename IO, typename Alloc> inline bool
generic_request<IO, Alloc>::has_file(typename generic_request<IO, Alloc>::string_type const &name) const {
	return map_has(files_, name);
}

template <typename IO, typename Alloc> inline typename generic_request<IO, Alloc>::file_ptr_type const&
generic_request<IO, Alloc>::get_file(typename generic_request<IO, Alloc>::string_type const &name) const {
	typename file_map_type::const_iterator i = files_.find(name);
	if (files_.end() != i) {
		return i->second;
	}
	else throw runtime_error("no file named %s", name.c_str());
}

template <typename IO, typename Alloc> inline void
generic_request<IO, Alloc>::add_file(typename generic_request<IO, Alloc>::string_type const &name, typename generic_request<IO, Alloc>::file_ptr_type const &f) {
	typename file_map_type::iterator i = files_.find(name);
	if (files_.end() == i) {
		files_.insert(std::make_pair(name, f));
	}
	else throw runtime_error("multiple file named %s", name.c_str());
}

template <typename IO, typename Alloc> inline bool
generic_request<IO, Alloc>::has_header(typename generic_request<IO, Alloc>::string_type const &name) const {
	return map_has(vars_, make_var_name(name));
}

template <typename IO, typename Alloc> inline typename generic_request<IO, Alloc>::string_type const&
generic_request<IO, Alloc>::header(typename generic_request<IO, Alloc>::string_type const &name) const {
	return map_get(vars_, make_var_name(name));
}

template <typename IO, typename Alloc> inline void
generic_request<IO, Alloc>::parse() {
	request_parser<type> parser(*this);
	parser.parse(0);
}

template <typename IO, typename Alloc> inline typename generic_request<IO, Alloc>::buffer_type
generic_request<IO, Alloc>::read_post() {
	std::size_t size = content_length();
	if (buffer_.read(io_reader<IO>(io_), size) != size) {
		throw runtime_error("can not read %llu bytes of post", static_cast<unsigned long long>(size));
	}
	return make_range<char const*>(buffer_.begin(), buffer_.end());
}

template <typename IO, typename Alloc> inline void
generic_request<IO, Alloc>::store(char const *name) {

	std::ofstream file;
	file.exceptions(std::ios::badbit | std::ios::failbit);
	file.open(name);
	char const* const* env = io_.environ();
	for (std::size_t i = 0; static_cast<char const*>(0) != env[i]; ++i) {
		file << env[i] << "\r\n";
	}
	file << "\r\n";
	if (http_method::post == method_) {
		assert(!buffer_.empty());
		buffer_.write(file);
	}
}

template <typename IO, typename Alloc> inline typename generic_request<IO, Alloc>::string_type const&
generic_request<IO, Alloc>::pattern() const {
	return empty_string_;
}

template <typename IO, typename Alloc> template <typename Map> inline bool
generic_request<IO, Alloc>::map_has(Map const &m, typename generic_request<IO, Alloc>::string_type const &key) const {
	return m.find(key) != m.end();
}

template <typename IO, typename Alloc> template <typename Map> inline typename generic_request<IO, Alloc>::string_type const&
generic_request<IO, Alloc>::map_get(Map const &m, typename generic_request<IO, Alloc>::string_type const &key) const {
	typename Map::const_iterator i = m.find(key);
	return i != m.end() ? i->second : empty_string_;
}

template <typename IO> inline
io_reader<IO>::io_reader(typename io_reader<IO>::io_type &io) :
	io_(io)
{
}

template <typename IO> inline std::size_t
io_reader<IO>::operator () (char *buffer, std::size_t size) {
	return io_.read(buffer, size);
}

}} // namespaces

#endif // FASTCGI_DAEMON_DETAILS_GENERIC_REQUEST_HPP_INCLUDED
