AC_DEFUN([AX_BOOST_THREAD],
[
	AC_REQUIRE([AX_BOOST_PREFIX])
	ax_boost_thread_stored_libs="$LIBS"
	ax_boost_thread_stored_ldflags="$LDFLAGS"
	
	AX_BOOST_LIB([ax_boost_thread_lib], [boost_thread])
	LDFLAGS="$ax_boost_thread_stored_ldflags $BOOST_LDFLAGS"
	LIBS="$ax_boost_thread_stored_libs -l$ax_boost_thread_lib"
	
	ax_have_boost_thread="yes"
	AX_BOOST_HEADER([thread.hpp], [], [ax_have_boost_thread="no"])
	AX_BOOST_HEADER([thread/mutex.hpp], [], [ax_have_boost_thread="no"])
	AX_BOOST_HEADER([thread/condition.hpp], [], [ax_have_boost_thread="no"])
	AC_MSG_CHECKING([linkage with boost::thread])
	
	AC_LANG_SAVE()
	AC_LANG([C++])
	AC_LINK_IFELSE(
		[ AC_LANG_PROGRAM([#include <boost/thread.hpp>], [boost::thread_group g;]) ],
		[ AC_MSG_RESULT(yes) ],
		[ AC_MSG_RESULT(no); ax_have_boost_thread="no" ])
	AC_LANG_RESTORE()
	
	LDFLAGS="$ax_boost_thread_stored_ldflags"
	LIBS="$ax_boost_thread_stored_libs"

	if test "f$ax_have_boost_thread" = "fyes"; then
		ifelse([$1], , :, [$1])
		AC_SUBST([BOOST_THREAD_LDFLAGS], ["-l$ax_boost_thread_lib"])
	else
		ifelse([$2], , :, [$2])
	fi	
])
