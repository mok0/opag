/* This file provides macros for the functions in <ctype.h>.
   Copyright (C) 2003, 2004 Martin Dickopp

   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This file is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this file; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
   USA.  */

#ifndef HDR_CHARTYPE
#define HDR_CHARTYPE 1


#if STDC_HEADERS

# include <ctype.h>

# define ISLOWER(c)  islower (c)
# define ISUPPER(c)  isupper (c)
# define ISALPHA(c)  isalpha (c)
# define ISDIGIT(c)  isdigit (c)
# define ISALNUM(c)  isalnum (c)
# define ISXDIGIT(c) isxdigit (c)
# define ISPUNCT(c)  ispunct (c)
# define ISSPACE(c)  isspace (c)
# define ISGRAPH(c)  isgraph (c)
# define ISPRINT(c)  isprint (c)
# define ISCNTRL(c)  iscntrl (c)
# define TOLOWER(c)  tolower (c)
# define TOUPPER(c)  toupper (c)

#else

# define ISALPHA(c)  (ISUPPER (c) || ISLOWER (c))
# define ISDIGIT(c)  ('0' <= (c) && (c) <= '9')
# define ISALNUM(c)  (ISDIGIT(c) || ISALPHA(c))
# define ISXDIGIT(c) (ISDIGIT(c) || ('A' <= (c) && (c) <= 'F') || ('a' <= (c) && (c) <= 'f'))
# define ISSPACE(c)  ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\f' || (c) == '\r' || (c) == '\v')
# define TOUPPER(c)  (ISLOWER (c) ? 'A' + ((c) - 'a') : (c))
# define TOLOWER(c)  (ISUPPER (c) ? 'a' + ((c) - 'A') : (c))

# if (' ' & 0xff) == 0x20
#  define ISLOWER(c) ('a' <= (c) && (c) <= 'z')
#  define ISUPPER(c) ('A' <= (c) && (c) <= 'Z')
#  define ISPUNCT(c) (ISGRAPH (c) && !ISALNUM (c))
#  define ISGRAPH(c) (' ' < (c) && (c) < 0x7f)
#  define ISPRINT(c) (' ' <= (c) && (c) < 0x7f)
#  define ISCNTRL(c) ((0 <= (c) && (c) < ' ') || (c) == 0x7f)
# else
#  define ISLOWER(c) (('a' <= (c) && (c) <= 'i') || ('j' <= (c) && (c) <= 'r') || ('s' <= (c) && (c) <= 'z'))
#  define ISUPPER(c) (('A' <= (c) && (c) <= 'I') || ('J' <= (c) && (c) <= 'R') || ('S' <= (c) && (c) <= 'Z'))
#  define ISPUNCT(c) ((0x4a <= (c) && (c) <= '&') || ('!' <= (c) && (c) <= '/') || (0x6a <= (c) && (c) <= '?') \
                      || (0x79 <= (c) && (c) <= '"') || (c) == '~' || (c) == '{' || (c) == '}' || (c) == '\\')
#  define ISGRAPH(c) (ISALNUM (c) || ISPUNCT (c))
#  define ISPRINT(c) (ISGRAPH (c) || (c) == ' ')
#  define ISCNTRL(c) ((0 <= (c) && (c) < ' ') || (c) == 0xff)
# endif

#endif


#if STDC_HEADERS && HAVE_ISBLANK
# define ISBLANK(c) isblank (c)
#else
# define ISBLANK(c) ((c) == ' ' || (c) == '\t')
#endif


#endif
