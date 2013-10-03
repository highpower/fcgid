#ifndef _CONFIG_ACSETUP_HPP
#define _CONFIG_ACSETUP_HPP 1
 
/* config/acsetup.hpp. Generated automatically at end of configure. */
/* config/config.h.  Generated from config.h.in by configure.  */
/* config/config.h.in.  Generated from configure.ac by autoheader.  */

/* Define to 1 if you have the `argz_add' function. */
/* #undef HAVE_ARGZ_ADD */

/* Define to 1 if you have the `argz_append' function. */
/* #undef HAVE_ARGZ_APPEND */

/* Define to 1 if you have the `argz_count' function. */
/* #undef HAVE_ARGZ_COUNT */

/* Define to 1 if you have the `argz_create_sep' function. */
/* #undef HAVE_ARGZ_CREATE_SEP */

/* Define to 1 if you have the <argz.h> header file. */
/* #undef HAVE_ARGZ_H */

/* Define to 1 if you have the `argz_insert' function. */
/* #undef HAVE_ARGZ_INSERT */

/* Define to 1 if you have the `argz_next' function. */
/* #undef HAVE_ARGZ_NEXT */

/* Define to 1 if you have the `argz_stringify' function. */
/* #undef HAVE_ARGZ_STRINGIFY */

/* Define to 1 if you have the `closedir' function. */
#ifndef FASTCGI_DAEMON_HAVE_CLOSEDIR 
#define FASTCGI_DAEMON_HAVE_CLOSEDIR  1 
#endif

/* Define to 1 if you have the declaration of `cygwin_conv_path', and to 0 if
   you don't. */
/* #undef HAVE_DECL_CYGWIN_CONV_PATH */

/* Define to 1 if you have the <dirent.h> header file. */
#ifndef FASTCGI_DAEMON_HAVE_DIRENT_H 
#define FASTCGI_DAEMON_HAVE_DIRENT_H  1 
#endif

/* Define if you have the GNU dld library. */
/* #undef HAVE_DLD */

/* Define to 1 if you have the <dld.h> header file. */
/* #undef HAVE_DLD_H */

/* Define to 1 if you have the `dlerror' function. */
#ifndef FASTCGI_DAEMON_HAVE_DLERROR 
#define FASTCGI_DAEMON_HAVE_DLERROR  1 
#endif

/* Define to 1 if you have the <dlfcn.h> header file. */
#ifndef FASTCGI_DAEMON_HAVE_DLFCN_H 
#define FASTCGI_DAEMON_HAVE_DLFCN_H  1 
#endif

/* Define to 1 if you have the <dl.h> header file. */
/* #undef HAVE_DL_H */

/* Define if you have the _dyld_func_lookup function. */
/* #undef HAVE_DYLD */

/* Define to 1 if the system has the type `error_t'. */
/* #undef HAVE_ERROR_T */

/* Define to 1 if you want to compile fastcgi module */
#ifndef FASTCGI_DAEMON_HAVE_FASTCGI 
#define FASTCGI_DAEMON_HAVE_FASTCGI  1 
#endif

/* Define to 1 if you have the <inttypes.h> header file. */
#ifndef FASTCGI_DAEMON_HAVE_INTTYPES_H 
#define FASTCGI_DAEMON_HAVE_INTTYPES_H  1 
#endif

/* Define if you have the libdl library or equivalent. */
#ifndef FASTCGI_DAEMON_HAVE_LIBDL 
#define FASTCGI_DAEMON_HAVE_LIBDL  1 
#endif

/* Define if libdlloader will be built on this platform */
#ifndef FASTCGI_DAEMON_HAVE_LIBDLLOADER 
#define FASTCGI_DAEMON_HAVE_LIBDLLOADER  1 
#endif

/* Define this if a modern libltdl is already installed */
#ifndef FASTCGI_DAEMON_HAVE_LTDL 
#define FASTCGI_DAEMON_HAVE_LTDL  1 
#endif

/* Define to 1 if you have the <mach-o/dyld.h> header file. */
#ifndef FASTCGI_DAEMON_HAVE_MACH_O_DYLD_H 
#define FASTCGI_DAEMON_HAVE_MACH_O_DYLD_H  1 
#endif

/* Define to 1 if you have the <memory.h> header file. */
#ifndef FASTCGI_DAEMON_HAVE_MEMORY_H 
#define FASTCGI_DAEMON_HAVE_MEMORY_H  1 
#endif

/* Define to 1 if you have the `opendir' function. */
#ifndef FASTCGI_DAEMON_HAVE_OPENDIR 
#define FASTCGI_DAEMON_HAVE_OPENDIR  1 
#endif

/* Define if libtool can extract symbol lists from object files. */
#ifndef FASTCGI_DAEMON_HAVE_PRELOADED_SYMBOLS 
#define FASTCGI_DAEMON_HAVE_PRELOADED_SYMBOLS  1 
#endif

/* Define to 1 if you have the `readdir' function. */
#ifndef FASTCGI_DAEMON_HAVE_READDIR 
#define FASTCGI_DAEMON_HAVE_READDIR  1 
#endif

/* Define if you have the shl_load function. */
/* #undef HAVE_SHL_LOAD */

/* Define to 1 if you have the <stdint.h> header file. */
#ifndef FASTCGI_DAEMON_HAVE_STDINT_H 
#define FASTCGI_DAEMON_HAVE_STDINT_H  1 
#endif

/* Define to 1 if you have the <stdlib.h> header file. */
#ifndef FASTCGI_DAEMON_HAVE_STDLIB_H 
#define FASTCGI_DAEMON_HAVE_STDLIB_H  1 
#endif

/* Define to 1 if you have the <strings.h> header file. */
#ifndef FASTCGI_DAEMON_HAVE_STRINGS_H 
#define FASTCGI_DAEMON_HAVE_STRINGS_H  1 
#endif

/* Define to 1 if you have the <string.h> header file. */
#ifndef FASTCGI_DAEMON_HAVE_STRING_H 
#define FASTCGI_DAEMON_HAVE_STRING_H  1 
#endif

/* Define to 1 if you have the `strlcat' function. */
#ifndef FASTCGI_DAEMON_HAVE_STRLCAT 
#define FASTCGI_DAEMON_HAVE_STRLCAT  1 
#endif

/* Define to 1 if you have the `strlcpy' function. */
#ifndef FASTCGI_DAEMON_HAVE_STRLCPY 
#define FASTCGI_DAEMON_HAVE_STRLCPY  1 
#endif

/* Define to 1 if you have strncasecmp function on your platform */
#ifndef FASTCGI_DAEMON_HAVE_STRNCASEMP 
#define FASTCGI_DAEMON_HAVE_STRNCASEMP  1 
#endif

/* Define to 1 if you have strncmp function on your platform */
#ifndef FASTCGI_DAEMON_HAVE_STRNCMP 
#define FASTCGI_DAEMON_HAVE_STRNCMP  1 
#endif

/* Define to 1 if you have the <sys/dl.h> header file. */
/* #undef HAVE_SYS_DL_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
#ifndef FASTCGI_DAEMON_HAVE_SYS_STAT_H 
#define FASTCGI_DAEMON_HAVE_SYS_STAT_H  1 
#endif

/* Define to 1 if you have the <sys/types.h> header file. */
#ifndef FASTCGI_DAEMON_HAVE_SYS_TYPES_H 
#define FASTCGI_DAEMON_HAVE_SYS_TYPES_H  1 
#endif

/* Define to 1 if you have the <unistd.h> header file. */
#ifndef FASTCGI_DAEMON_HAVE_UNISTD_H 
#define FASTCGI_DAEMON_HAVE_UNISTD_H  1 
#endif

/* This value is set to 1 to indicate that the system argz facility works */
/* #undef HAVE_WORKING_ARGZ */

/* Define if the OS needs help to load dependent libraries for dlopen(). */
/* #undef LTDL_DLOPEN_DEPLIBS */

/* Define to the system default library search path. */
#ifndef FASTCGI_DAEMON_LT_DLSEARCH_PATH 
#define FASTCGI_DAEMON_LT_DLSEARCH_PATH  "/usr/local/lib:/lib:/usr/lib" 
#endif

/* The archive extension */
#ifndef FASTCGI_DAEMON_LT_LIBEXT 
#define FASTCGI_DAEMON_LT_LIBEXT  "a" 
#endif

/* The archive prefix */
#ifndef FASTCGI_DAEMON_LT_LIBPREFIX 
#define FASTCGI_DAEMON_LT_LIBPREFIX  "lib" 
#endif

/* Define to the extension used for runtime loadable modules, say, ".so". */
#ifndef FASTCGI_DAEMON_LT_MODULE_EXT 
#define FASTCGI_DAEMON_LT_MODULE_EXT  ".so" 
#endif

/* Define to the name of the environment variable that determines the run-time
   module search path. */
#ifndef FASTCGI_DAEMON_LT_MODULE_PATH_VAR 
#define FASTCGI_DAEMON_LT_MODULE_PATH_VAR  "DYLD_LIBRARY_PATH" 
#endif

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#ifndef FASTCGI_DAEMON_LT_OBJDIR 
#define FASTCGI_DAEMON_LT_OBJDIR  ".libs/" 
#endif

/* Define to the shared library suffix, say, ".dylib". */
#ifndef FASTCGI_DAEMON_LT_SHARED_EXT 
#define FASTCGI_DAEMON_LT_SHARED_EXT  ".dylib" 
#endif

/* Define if dlsym() requires a leading underscore in symbol names. */
/* #undef NEED_USCORE */

/* Name of package */
#ifndef FASTCGI_DAEMON_PACKAGE 
#define FASTCGI_DAEMON_PACKAGE  "fastcgi-daemon" 
#endif

/* Define to the address where bug reports for this package should be sent. */
#ifndef FASTCGI_DAEMON_PACKAGE_BUGREPORT 
#define FASTCGI_DAEMON_PACKAGE_BUGREPORT  "highpower@rambler.ru" 
#endif

/* Define to the full name of this package. */
#ifndef FASTCGI_DAEMON_PACKAGE_NAME 
#define FASTCGI_DAEMON_PACKAGE_NAME  "fastcgi-daemon" 
#endif

/* Define to the full name and version of this package. */
#ifndef FASTCGI_DAEMON_PACKAGE_STRING 
#define FASTCGI_DAEMON_PACKAGE_STRING  "fastcgi-daemon 4.0.0" 
#endif

/* Define to the one symbol short name of this package. */
#ifndef FASTCGI_DAEMON_PACKAGE_TARNAME 
#define FASTCGI_DAEMON_PACKAGE_TARNAME  "fastcgi-daemon" 
#endif

/* Define to the home page for this package. */
#ifndef FASTCGI_DAEMON_PACKAGE_URL 
#define FASTCGI_DAEMON_PACKAGE_URL  "" 
#endif

/* Define to the version of this package. */
#ifndef FASTCGI_DAEMON_PACKAGE_VERSION 
#define FASTCGI_DAEMON_PACKAGE_VERSION  "4.0.0" 
#endif

/* Define to 1 if you have the ANSI C header files. */
#ifndef FASTCGI_DAEMON_STDC_HEADERS 
#define FASTCGI_DAEMON_STDC_HEADERS  1 
#endif

/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
# define _ALL_SOURCE 1
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif
/* Enable threading extensions on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
# define _TANDEM_SOURCE 1
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# define __EXTENSIONS__ 1
#endif


/* Version number of package */
#ifndef FASTCGI_DAEMON_VERSION 
#define FASTCGI_DAEMON_VERSION  "4.0.0" 
#endif

/* Define to 1 if on MINIX. */
/* #undef _MINIX */

/* Define to 2 if the system does not provide POSIX.1 features except with
   this defined. */
/* #undef _POSIX_1_SOURCE */

/* Define to 1 if you need to in order for `stat' and other things to work. */
/* #undef _POSIX_SOURCE */

/* Define so that glibc/gnulib argp.h does not typedef error_t. */
#ifndef FASTCGI_DAEMON___error_t_defined 
#define FASTCGI_DAEMON___error_t_defined  1 
#endif

/* Define to a type to use for `error_t' if it is not otherwise available. */
#ifndef _fastcgi_daemon_error_t 
#define _fastcgi_daemon_error_t  int 
#endif
 
/* once: _CONFIG_ACSETUP_HPP */
#endif
