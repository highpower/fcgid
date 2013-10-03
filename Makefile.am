SUBDIRS = include libltdl library main

if HAVE_UNIT_TESTS
SUBDIRS += tests
endif

ACLOCAL_AMFLAGS = -I config
LIBTOOLIZE_OPTIONS = --force --copy --ltdl

EXTRA_DIST = autogen.sh AUTHORS ChangeLog COPYING INSTALL NEWS README \
	config/ax_boost_header.m4 config/ax_boost_lib.m4 \
	config/ax_boost_prefix.m4 config/ax_boost_thread.m4 \
	config/ax_boost_unit_test.m4 config/ax_check_compiler_flags.m4 \
	config/ax_check_fastcgi.m4 config/ax_prefix_config_h.m4
