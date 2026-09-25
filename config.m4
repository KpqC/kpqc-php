PHP_ARG_ENABLE([kpqc],
  [whether to enable KpqC support],
  [AS_HELP_STRING([--enable-kpqc], [Enable KpqC support])],
  [yes])

if test "$PHP_KPQC" != "no"; then
  AC_PATH_PROG([CMAKE], [cmake], [no])
  AS_IF([test "$CMAKE" = "no"], [
    AC_MSG_ERROR([cmake 3.20 or newer is required to build KpqC])
  ])
  PHP_SUBST([CMAKE])

  AC_MSG_CHECKING([for cmake 3.20 or newer])
  KPQC_CMAKE_VERSION=`$CMAKE --version | sed -n '1p' | cut -d ' ' -f 3`
  AS_VERSION_COMPARE([$KPQC_CMAKE_VERSION], [3.20], [
    AC_MSG_RESULT([no ($KPQC_CMAKE_VERSION)])
    AC_MSG_ERROR([cmake 3.20 or newer is required to build KpqC])
  ], [
    AC_MSG_RESULT([yes ($KPQC_CMAKE_VERSION)])
  ], [
    AC_MSG_RESULT([yes ($KPQC_CMAKE_VERSION)])
  ])

  PHP_REQUIRE_CXX()
  PHP_ADD_INCLUDE([$srcdir/native/kpqc/include])
  PHP_NEW_EXTENSION(
    [kpqc],
    [ext/kpqc.cpp],
    [$ext_shared],,
    [-std=c++17 -Wall -Wextra -Wpedantic],
    [cxx])

  KPQC_SHARED_LIBADD="$KPQC_SHARED_LIBADD $ext_builddir/.pie-native/libkpqc.a"
  AS_CASE([$host_os], [linux*], [PHP_ADD_LIBRARY([m],, [KPQC_SHARED_LIBADD])])
  PHP_SUBST([KPQC_SHARED_LIBADD])
  PHP_ADD_MAKEFILE_FRAGMENT([Makefile.frag])
fi
