#include "acsetup.hpp"

#include <boost/test/unit_test.hpp>

#include "mock_handler.hpp"

#include "fastcgi-daemon/typedefs.hpp"
#include "fastcgi-daemon/http_error.hpp"
#include "fastcgi-daemon/http_status.hpp"
#include "fastcgi-daemon/details/urltree.hpp"
#include "fastcgi-daemon/details/null_logger.hpp"
#include "fastcgi-daemon/details/default_invoker.hpp"

namespace fcgid { namespace tests {

bool
is_not_found(http_error const &e) {
	return e.status() == http_status::not_found;
}

BOOST_AUTO_TEST_SUITE(urltree_test)

BOOST_AUTO_TEST_CASE(test_existent) {
	
	using namespace details;

	null_logger log;
	urltree<default_invoker<mock_handler>::type> tree;
	tree.add(url_type("/abc/def/ghi"), queue_name_type(0), mock_handler("the name"));
	
	boost::shared_ptr<mock_context> ctx(new mock_context("/abc/def/ghi"));
	tree.handle(ctx, log);
	BOOST_CHECK_EQUAL("the name", ctx->response().content());
}

BOOST_AUTO_TEST_CASE(test_wildcard) {

	using namespace details;

	null_logger log;
	urltree<default_invoker<mock_handler>::type> tree;
	tree.add(url_type("/abc/*/ghi"), queue_name_type(0), mock_handler("the name"));
	tree.add(url_type("/abc/def/*"), queue_name_type(0), mock_handler("the other name"));
	
	boost::shared_ptr<mock_context> ctx(new mock_context("/abc/def/ghi"));
	tree.handle(ctx, log);
	BOOST_CHECK_EQUAL("the other name", ctx->response().content());
}

BOOST_AUTO_TEST_CASE(test_existent_wildcard) {
	
	using namespace details;

	null_logger log;
	urltree<default_invoker<mock_handler>::type> tree;
	tree.add(url_type("/abc/def/*"), queue_name_type(0), mock_handler("the name"));
	tree.add(url_type("/abc/def/ghi"), queue_name_type(0), mock_handler("the other name"));
	
	boost::shared_ptr<mock_context> ctx(new mock_context("/abc/def/ghi"));
	tree.handle(ctx, log);
	BOOST_CHECK_EQUAL("the other name", ctx->response().content());
}

BOOST_AUTO_TEST_CASE(test_nonexistent) {

	using namespace details;
	
	null_logger log;
	urltree<default_invoker<mock_handler>::type> tree;
	tree.add(url_type("/abc/def/*"), queue_name_type(0), mock_handler("the name"));

	boost::shared_ptr<mock_context> ctx;
	ctx.reset(new mock_context("/"));
	
	BOOST_CHECK_EXCEPTION(tree.handle(ctx, log), http_error, is_not_found);
}

BOOST_AUTO_TEST_SUITE_END();

}} // namespaces
