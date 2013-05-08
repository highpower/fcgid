check_PROGRAMS = fastcgi-daemon-tests

fastcgi_daemon_tests_LDADD = ${top_srcdir}/library/libfastcgi-daemon.la
fastcgi_daemon_tests_SOURCES = main.cpp mock_io.cpp mock_request.cpp \
	mock_response.cpp test_utils.cpp \
	cookie_test.cpp distinct_test.cpp functors_test.cpp http_date_test.cpp \
	http_method_test.cpp iter_streambuf_test.cpp iterator_test.cpp \
	line_end_test.cpp parser_test.cpp range_test.cpp request_test.cpp \
	response_test.cpp strings_test.cpp tokenizer_test.cpp urlencode_test.cpp \
	urltree_test.cpp

TESTS = fastcgi-daemon-tests

AM_CXXFLAGS = -pthread
AM_CPPFLAGS = -I${top_srcdir}/include -I${top_srcdir}/config @BOOST_CPPFLAGS@
AM_LDFLAGS = @BOOST_LDFLAGS@ @BOOST_THREAD_LDFLAGS@ @BOOST_UNIT_TEST_LDFLAGS@

noinst_HEADERS = mock_io.hpp mock_iterator.hpp mock_request.hpp \
	mock_response.hpp test_utils.hpp
