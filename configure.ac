#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])
AC_INIT([teamrep], [0.0.1], [stefano.braghin@gmail.com])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([src/config.hpp])

AC_CANONICAL_HOST

# Checks for programs.
AC_PROG_CXX([g++ clang++])

# Checks for libraries.

# Checks for header files.
AC_LANG_PUSH([C++])
AC_CHECK_HEADERS([boost/format.hpp], [],
    [AC_MSG_ERROR(You need the Boost libraries.)])
AC_CHECK_HEADERS([boost/graph/breadth_first_search.hpp], [],
    [AC_MSG_ERROR(You need the Boost libraries.)])
AC_CHECK_HEADERS([boost/graph/random.hpp], [],
    [AC_MSG_ERROR(You need the Boost libraries.)])
AC_CHECK_HEADERS([boost/graph/topological_sort.hpp], [],
    [AC_MSG_ERROR(You need the Boost libraries.)])
AC_CHECK_HEADERS([boost/graph/visitors.hpp], [],
    [AC_MSG_ERROR(You need the Boost libraries.)])
AC_CHECK_HEADERS([boost/tokenizer.hpp], [],
    [AC_MSG_ERROR(You need the Boost libraries.)])
AC_CHECK_HEADERS([boost/graph/adjacency_list.hpp], [],
    [AC_MSG_ERROR(You need the Boost libraries.)])
AC_CHECK_HEADERS([boost/graph/graph_traits.hpp], [],
    [AC_MSG_ERROR(You need the Boost libraries.)])
AC_CHECK_HEADERS([boost/random/mersenne_twister.hpp], [],
    [AC_MSG_ERROR(You need the Boost libraries.)])
AC_LANG_POP([C++])

# Checks for typedefs, structures, and compiler characteristics.
case $CXX in
     clang++)
	AC_MSG_NOTICE([Using clang++ on $host_os])
	case $host_os in
	     *darwin*10.*.*)
		AC_MSG_NOTICE([Applying special configuration for clang])
		CXXFLAGS="$CXXFLAGS -stdlib=c++11 -U__STRICT_ANSI__ -I/opt/local/libexec/llvm-3.3/lib/c++/v1"
		;;
	esac
	;;
esac

# Checks for library functions.

AM_INIT_AUTOMAKE([1.11 foreign no-define])

AC_CONFIG_FILES([Makefile
	src/Makefile])

AC_OUTPUT
