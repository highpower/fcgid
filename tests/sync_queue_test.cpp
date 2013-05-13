#include "acsetup.hpp"

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/test/unit_test.hpp>

#include "fastcgi-daemon/details/sync_queue.hpp"

namespace fcgid { namespace tests { 

void
use_queue(details::unbounded_sync_queue<size_t> &queue, std::size_t count) {
	for (std::size_t i = 0; i < count; ++i) {
		queue.push(i);
	}
}

BOOST_AUTO_TEST_SUITE(sync_queue_test) 

BOOST_AUTO_TEST_CASE(test_queue) {
	
	using namespace details;
	
	unbounded_sync_queue<size_t> queue;
	boost::thread th(boost::bind(&use_queue, boost::ref(queue), 1000));
	
	for (std::size_t i = 0; i < 1000; ++i) {
		std::pair<std::size_t, bool> res = queue.pop();
		BOOST_CHECK_EQUAL(true, res.second);
		BOOST_CHECK_EQUAL(i, res.first);
	}
	th.join();
}


BOOST_AUTO_TEST_SUITE_END();

}} // namespaces
