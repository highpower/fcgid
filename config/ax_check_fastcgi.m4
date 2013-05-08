AC_DEFUN([AX_CHECK_FASTCGI],
[
	ax_have_fastcgi="yes"
	AC_CHECK_HEADER([fcgiapp.h], [], [ax_have_fastcgi="no"])
	
	ax_check_fastcgi_stored_libs="$LIBS"
	ax_check_fastcgi_stored_ldflags="$LDFLAGS"
	
	LDFLAGS="$ax_check_fastcgi_stored_ldflags"
	LIBS="$ax_check_fastcgi_stored_libs -lfcgi"

	AC_MSG_CHECKING([linkage with libfcgi])
	AC_LINK_IFELSE(
		[AC_LANG_PROGRAM([#include <fcgiapp.h>], [FCGX_Init();])],
		[AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no]); ax_have_fastcgi="no"])
	
	LDFLAGS="$ax_check_fastcgi_stored_ldflags"
	LIBS="$ax_check_fastcgi_stored_libs"

	if test "f$ax_have_fastcgi" = "fyes"; then
		AC_SUBST([FCGI_LIBS], [-lfcgi])
		AC_DEFINE([HAVE_FASTCGI], 1, [Define to 1 if you want to compile fastcgi module])
		ifelse([$1], , :, [$1])
	else
		ifelse([$2], , :, [$2])
	fi
	AM_CONDITIONAL(HAVE_FASTCGI, [ test "f$ax_have_fastcgi" = "fyes" ])
])
