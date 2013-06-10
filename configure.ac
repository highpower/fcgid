AC_INIT([fastcgi-daemon], [4.0.0], [highpower@rambler.ru])
AM_INIT_AUTOMAKE(1.9)

AC_GNU_SOURCE
AC_CONFIG_MACRO_DIR([config])
AC_CONFIG_HEADERS(config/config.h)

AC_PROG_CXX
AC_PROG_CXXCPP
AC_LANG([C++])

AC_PROG_LIBTOOL
LT_INIT([dlopen])
LT_CONFIG_LTDL_DIR([libltdl])
LTDL_INIT

AM_MAINTAINER_MODE

CPPFLAGS="$CPPFLAGS -pthread"
AX_PREFIX_CONFIG_H(config/acsetup.hpp)
for i in -W -Wall -Wextra -Wlint -fexceptions -frtti; do
	AX_CHECK_COMPILER_FLAGS([$i], [CXXFLAGS="$CXXFLAGS $i"], [])
done

AC_CHECK_FUNC([strncmp], [AC_DEFINE([HAVE_STRNCMP], [1], [Define to 1 if you have strncmp function on your platform])], [])
AC_CHECK_FUNC([strncasecmp], [AC_DEFINE([HAVE_STRNCASEMP], [1], [Define to 1 if you have strncasecmp function on your platform])], [])

AX_CHECK_FASTCGI([], [AC_MSG_ERROR(libfcgi is not found)])

AX_BOOST_PREFIX([], [AC_MSG_ERROR(boost installation is not found)])
AX_BOOST_THREAD([], [AC_MSG_ERROR(boost::thread is not found)])

ax_boost_ok="yes"
AX_BOOST_HEADER([mpl/list.hpp], [], [ax_boost_ok="no"])
AX_BOOST_HEADER([type_traits.hpp], [], [ax_boost_ok="no"])
AX_BOOST_HEADER([static_assert.hpp], [], [ax_boost_ok="no"])
AX_BOOST_HEADER([shared_ptr.hpp], [], [ax_boost_ok="no"])
AX_BOOST_HEADER([intrusive_ptr.hpp], [], [ax_boost_ok="no"])
AX_BOOST_HEADER([functional/hash.hpp], [], [ax_boost_ok="no"])
AX_BOOST_HEADER([iterator/iterator_facade.hpp], [], [ax_boost_ok="no"])

if test "f$ax_boost_ok" != "fyes"; then
	AC_MSG_ERROR([required boost headers not found])
fi

ax_have_unit_tests="yes"
AC_ARG_ENABLE([unit-tests], AS_HELP_STRING([--enable-unit-tests], [enables unit tests compilation and run @<:@default=yes@:>@]), 
[
	if test "f$enableval" != "fyes"; then
		ax_have_unit_tests="no"
	else
		AX_BOOST_UNIT_TEST([], [ax_have_unit_tests="no"])
	fi
])

AM_CONDITIONAL(HAVE_UNIT_TESTS, [test "f$ax_have_unit_tests" = "fyes"])

AC_SUBST(INCLTDL)
AC_SUBST(LIBLTDL)

AC_CONFIG_FILES([Makefile include/Makefile include/fastcgi-daemon/Makefile 
	library/Makefile main/Makefile tests/Makefile])

AC_OUTPUT
