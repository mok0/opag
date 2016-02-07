## Copyright (C) 2002, 2003 Martin Dickopp
##
## This file is free software; the copyright holder gives unlimited
## permission to copy and/or distribute it, with or without
## modifications, as long as this notice is preserved.
##
## This file is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY, to the extent permitted by law; without
## even the implied warranty of MERCHANTABILITY or FITNESS FOR A
## PARTICULAR PURPOSE.


# MD_PATH_PROG(PROGRAM)
# ---------------------
# Set PROGRAM to the path to PROGRAM, or to `missing' if the program is
# not available.
#
AC_DEFUN([MD_PATH_PROG],
[AS_VAR_PUSHDEF([md_Program], [md_path_$1])dnl
AS_VAR_SET(md_Program, [])
AC_ARG_WITH([$1],
             AC_HELP_STRING([--with-$1=PROGRAM], [use PROGRAM as $1 executable]),
[case $withval in
yes)
  ;;
no)
  AS_VAR_SET(md_Program, missing)
  ;;
*)
  AS_VAR_SET(md_Program, ["$withval"])
  ;;
esac])
AS_IF([test "x[]AS_VAR_GET(md_Program)" != x],
      [AS_TR_CPP([$1])="AS_VAR_GET(md_Program)"],
      [AC_PATH_PROG(AS_TR_CPP([$1]), [$1], missing)])dnl
AS_VAR_POPDEF([md_Program])dnl
])# MD_PATH_PROG


# MD_CHECK_TYPE_SYS_ERRLIST
# -------------------------
# Set SYS_ERRLIST_TYPE to the type of sys_errlist [0].
#
AC_DEFUN([MD_CHECK_TYPE_SYS_ERRLIST],
[AC_CACHE_CHECK([for type of sys_errlist elements], [md_cv_var_type_sys_errlist],
[md_cv_var_type_sys_errlist='char *'
for md_type_sys_errlist in 'const char *const' 'const char *' 'char *const'; do
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([AC_INCLUDES_DEFAULT],
                                     [extern $md_type_sys_errlist sys_errlist @<:@@:>@;])],
                    [md_cv_var_type_sys_errlist="$md_type_sys_errlist"; break])
done])
AC_DEFINE_UNQUOTED(SYS_ERRLIST_TYPE, [$md_cv_var_type_sys_errlist], [Define to the type of `sys_errlist' elements.])dnl
])# MD_CHECK_TYPE_SYS_ERRLIST
