#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([getttfb], [0.1], [therealbill@me.com])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([getttfb.c])
AC_CONFIG_HEADERS([config.h])
#AM_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
#AC_SEARCH_LIBS([curl])

# Checks for header files.
AC_CHECK_HEADERS([string.h sys/time.h curl/curl.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([gettimeofday])
PKG_CHECK_MODULES([DEPS], [libcurl >= 7.29.0])


# Options
#AC_ARG_ENABLE([redis], [--enable-redis Upload into local Redis instance], [redis=${enable_val}], [redis=no] )

# Check for redis
AC_ARG_WITH([redis],
		AS_HELP_STRING([--without-redis], [disable hiredis (default is no)]),
		[with_redis=no],
		[with_redis=${enable_val}]
		AC_DEFINE(USEREDIS,,[define if --without-redis option is not set])
)

AS_IF([test "x$with_redis" != xno],
	AC_CHECK_LIB(hiredis, redisAsyncConnect, [], AC_MSG_ERROR([hiredis library not found]))
	echo "/************************************************************/"
	echo " *                                                          * "
	echo " *   Redis enabled                                          * "
	echo " *   Be sure to run with a local redis instance running     * "
	echo " *                                                          * "
	echo "/************************************************************/"
#define USEREDIS=yes
)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
