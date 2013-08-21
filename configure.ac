#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])
AC_INIT([teamrep], [0.0.1], [stefano.braghin@deri.org])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([src/config.hpp])

AC_CANONICAL_HOST



# Checks for programs.
AC_PROG_CXX([g++ clang++])
case $CXX in
     clang++)
	AC_MSG_NOTICE([Using clang++ on $host_os])
	case $host_os in
	     *darwin*10.*.*)
		AC_MSG_NOTICE([adding -U because os is $host_os])
		CXXFLAGS="$CXXFLAGS -stdlib=c++11 -U__STRICT_ANSI__ -I/opt/local/libexec/llvm-3.3/lib/c++/v1"
		;;
	esac
	;;
esac

# Checks for libraries.


# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.



AM_INIT_AUTOMAKE([1.11 foreign no-define])

AC_CONFIG_FILES([Makefile
	src/Makefile])

AC_OUTPUT
