#include "acsetup.hpp"
#include "fastcgi-daemon/server.hpp"

#include <algorithm>

#include "fastcgi-daemon/details/server_impl.hpp"
#include "fastcgi-daemon/details/standard_handler.hpp"

namespace fcgid {

server::server() :
	impl_(new impl_type())
{
}

server::~server() {
}

server::server(server const &other) :
	impl_(other.impl_)
{
}

server&
server::operator = (server const &other) {
	server copy(other);
	swap(copy);
	return *this;
}

void
server::swap(server &other) throw () {
	using std::swap;
	swap(impl_, other.impl_);
}

void
server::stop() {
	impl_->stop();
}

void
server::start(thread_count_type nthreads) {
	impl_->start(nthreads);
}

void
server::listen(endpoint const &ep) {
	impl_->listen(ep);
}

void
server::attach_logger(boost::shared_ptr<logger> const &log) {
	impl_->attach_logger(log);
}

void
server::create_request_queue(queue_name_type, thread_count_type nthreads) {
}

void
server::attach_handler(url_type url, queue_name_type queue, boost::shared_ptr<request_handler> const &handler, http_method_mask const &mask) {
	details::standard_handler sth(handler, mask);
}

} // namespaces
