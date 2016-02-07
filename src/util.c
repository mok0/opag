/* This file is part of opag, an option parser generator.
   Copyright (C) 2002, 2004 Martin Dickopp

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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include "stdinc.h"

#include <stdio.h>

#if HAVE_UNISTD_H
# include <unistd.h>
#endif

#if !STDC_HEADERS && HAVE_MALLOC_H
# include <malloc.h>
#endif
#if !HAVE_DECL_MALLOC
extern void *malloc (), *realloc ();
#endif

#include "chartype.h"
#include "util.h"

#ifndef STDERR_FILENO
# define STDERR_FILENO 2
#endif



/* Name under which the program has been invoked.  */
const char *invocation_name;



/* malloc wrapper.  Terminates the program if memory allocation fails.  */
void *
xmalloc (const size_t size)
{
    void *const ptr = malloc (size);
    if (ptr == 0)
        mem_alloc_failed ();
    return ptr;
}



/* realloc wrapper.  Terminates the program if memory allocation fails.  */
void *
xrealloc (void *ptr, const size_t size)
{
    ptr = realloc (ptr, size);
    if (ptr == 0)
        mem_alloc_failed ();
    return ptr;
}



/* Report a failure to allocate memory and terminate the program.
   This function does not use fprintf to avoid further memory allocation attempts.  */
void
mem_alloc_failed (void)
{
    {
        const char *ptr = invocation_name;
        size_t len = strlen (ptr);

        while (len > 0)
        {
            const ssize_t chars_written = write (STDERR_FILENO, ptr, len);
            /* If writing to standard error fails, terminate right away.  */
            if (chars_written == -1)
                exit (2);
            len -= chars_written;
            ptr += chars_written;
        }
    }

    {
        static const char message [] = ": out of memory\n";
        const char *ptr = message;
        size_t len = numof (message) - 1;

        while (len > 0)
        {
            const ssize_t chars_written = write (STDERR_FILENO, ptr, len);
            /* If writing to standard error fails, terminate right away.  */
            if (chars_written == -1)
                exit (2);
            len -= chars_written;
            ptr += chars_written;
        }
    }

    exit (2);
}



/* Test if a string is a valid C identifier.  */
int
c_identifier (const char *s)
{
    if (!ISALPHA ((unsigned char)*s) && *s != '_')
        return 0;

    while (*++s != '\0')
        if (!ISALNUM ((unsigned char)*s) && *s != '_')
            return 0;

    return 1;
}



/* Test if a string is a valid C++ identifier (possibly scope qualified).  */
int
cxx_scoped_identifier (const char *s)
{
    if (*s != ':')
        goto no_scope_prefix;

   while (1)
    {
        if (*++s != ':')
            return 0;
        ++s;

      no_scope_prefix:
        if (!ISALPHA ((unsigned char)*s) && *s != '_')
            return 0;

        while (*++s != ':')
        {
            if (*s == '\0')
                return 1;
            else if (!ISALNUM ((unsigned char)*s) && *s != '_')
                return 0;
        }
    }
}
