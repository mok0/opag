/* This file is part of opag, an option parser generator.
   Copyright (C) 2004 Martin Dickopp

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
   USA.  */

#ifndef HDR_STDINC
#define HDR_STDINC 1


/* Default includes.  */
#if HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif
#if STDC_HEADERS
# include <stdlib.h>
# include <stddef.h>
#else
# if HAVE_STDLIB_H
#  include <stdlib.h>
# endif
#endif
#if HAVE_STRING_H
# if !STDC_HEADERS && HAVE_MEMORY_H
#  include <memory.h>
# endif
# include <string.h>
#endif
#if HAVE_STRINGS_H
# include <strings.h>
#endif
#if HAVE_INTTYPES_H
# include <inttypes.h>
#else
# if HAVE_STDINT_H
#  include <stdint.h>
# endif
#endif

/* GCC attributes.  */
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 5)
# define gcc_attr_const    __attribute__ ((const))
# define gcc_attr_noreturn __attribute__ ((noreturn))
#else
# define gcc_attr_const
# define gcc_attr_noreturn
#endif
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 96)
# define gcc_attr_malloc __attribute__ ((malloc))
# define gcc_attr_pure   __attribute__ ((pure))
#else
# define gcc_attr_malloc
# define gcc_attr_pure
#endif
#if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3)
# define gcc_attr_nonnull(x) __attribute__ ((nonnull x))
#else
# define gcc_attr_nonnull(x)
#endif

/* Type definitions missing in the standard headers.  */
#if !HAVE_SIZE_T
typedef unsigned int size_t;
#endif

#if !HAVE_SSIZE_T
typedef int ssize_t;
#endif

/* Declarations missing in the standard headers.  */
#if !HAVE_DECL_STRCHR
extern char *strchr (), *strrchr ();
#endif

#if !HAVE_DECL_STRERROR
extern char *strerror ();
#endif


#endif
