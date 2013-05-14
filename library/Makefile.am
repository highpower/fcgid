lib_LTLIBRARIES = libfastcgi-daemon.la

libfastcgi_daemon_la_SOURCES = cookie.cpp endpoint.cpp endpoint_impl.cpp \
	fastcgi.cpp fatal_error.cpp file.cpp fileset.cpp generic_request.cpp \
	generic_response.cpp http_date.cpp http_method.cpp http_status.cpp \
	logger.cpp null_logger.cpp paramset.cpp post_buffer.cpp request.cpp \
	request_handler.cpp request_parser.cpp response.cpp runtime_error.cpp \
	server.cpp server_impl.cpp standard_handler.cpp statistics.cpp \
	statistics_impl.cpp sync_queue.cpp urlencode.cpp urltree.cpp

AM_CXXFLAGS = -pthread
AM_CPPFLAGS = -I${top_srcdir}/include -I${top_srcdir}/config \
	-DFASTCGI_DAEMON_LIBRARY @BOOST_CPPFLAGS@

AM_LDFLAGS = @BOOST_LDFLAGS@ @BOOST_THREAD_LDFLAGS@ @FCGI_LIBS@
