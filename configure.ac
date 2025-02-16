dnl Process this file with autoconf to produce configure.
dnl This file is part of opag, an option parser generator.
dnl Copyright (C) 2002, 2003, 2004, 2005 Martin Dickopp
dnl
dnl This file is free software; the copyright holder gives unlimited
dnl permission to copy and/or distribute it, with or without
dnl modifications, as long as this notice is preserved.
dnl
dnl This file is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY, to the extent permitted by law; without
dnl even the implied warranty of MERCHANTABILITY or FITNESS FOR A
dnl PARTICULAR PURPOSE.

AC_PREREQ([2.71])
AC_INIT([opag],[0.8.0],[martin@zero-based.org])
AC_CONFIG_SRCDIR([src/opag.c])
AC_COPYRIGHT([Copyright (C) 2002, 2003, 2004, 2005 Martin Dickopp])
AM_INIT_AUTOMAKE([gnu no-define std-options 1.9])
AC_CONFIG_HEADERS([config.h])

MD_PATH_PROG([opag])

AC_USE_SYSTEM_EXTENSIONS
AC_PROG_CC
if test "x$ac_cv_prog_cc_stdc" = xno; then
  AC_MSG_ERROR([an ANSI C compiler is required to build this package])
fi
AC_C_CONST
AC_C_INLINE

AC_CHECK_HEADERS([errno.h fcntl.h malloc.h sys/file.h], [], [], [AC_INCLUDES_DEFAULT])

AC_CHECK_DECLS([strerror, strchr], [], [], [AC_INCLUDES_DEFAULT])
AC_CHECK_DECLS([malloc], [], [], [AC_INCLUDES_DEFAULT
@%:@if !STDC_HEADERS && HAVE_MALLOC_H
@%:@ include <malloc.h>
@%:@endif])

AC_CHECK_TYPES([size_t, ssize_t], [], [], [AC_INCLUDES_DEFAULT])

AC_SEARCH_LIBS([remove], [posix])
AC_SEARCH_LIBS([strerror], [cposix])

AC_CHECK_FUNCS([isblank])

AC_REPLACE_FUNCS([qsort strerror])

if test "x$ac_cv_func_strerror" != xyes; then
  MD_CHECK_TYPE_SYS_ERRLIST
fi

AC_CONFIG_FILES([Makefile src/Makefile man/Makefile man/opag.1])
AC_OUTPUT
