sbin_PROGRAMS = fastcgi-daemon

fastcgi_daemon_SOURCES = main.cpp \
	dynamic_loader.cpp xml_config.cpp
	
fastcgi_daemon_LDADD = ${top_srcdir}/library/libfastcgi-daemon.la

AM_CXXFLAGS = -pthread
AM_CPPFLAGS = -I${top_srcdir}/include -I${top_srcdir}/config \
	@INCLTDL@ @BOOST_CPPFLAGS@ 
AM_LDFLAGS = @LIBLTDL@ @BOOST_LDFLAGS@ @BOOST_THREAD_LDFLAGS@

noinst_HEADERS = dynamic_loader.hpp xml_config.hpp 