/* This file is part of opag, an option parser generator.
   Copyright (C) 2003, 2004, 2005 Martin Dickopp

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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include "stdinc.h"

#include <stdio.h>

#if HAVE_FCNTL_H
# include <fcntl.h>
#else
# if HAVE_SYS_FILE_H
#  include <sys/file.h>
# endif
#endif

#if HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif

#if HAVE_UNISTD_H
# include <unistd.h>
#endif

#if HAVE_ERRNO_H
# include <errno.h>
#endif
#ifndef errno
extern int errno;
#endif

#include "util.h"
#include "fileread.h"

#ifndef STDIN_FILENO
# define STDIN_FILENO 0
#endif



/* Read a file into memory and set `filebuffer' accordingly.  If `filename'
   is a null pointer, standard input is read.  */
void
file_read (const char *const filename, struct filebuffer *const filebuffer)
{
    int fd;
    size_t buffer_len;



    /* Open file.  */
    if (filename != 0)
    {
        if ((fd = open (filename, O_RDONLY)) == -1)
        {
            fprintf (stderr, "%s: %s: %s\n", invocation_name, filename != 0 ? filename : "STDIN", strerror (errno));
            exit (2);
        }
    }
    else
        fd = STDIN_FILENO;


    /* Find out file length if possible, allocate buffer.  */
    {
        struct stat st;

        if (fstat (fd, &st) != -1)
        {
            buffer_len = (size_t)st.st_size;

            if (++buffer_len < 0x400)
                buffer_len = 0x400;
        }
        else
            buffer_len = 0x400;
    }

    filebuffer->ptr = xmalloc (buffer_len);
    filebuffer->len = 0;


    /* Read file into buffer.  */
    while (1)
    {
        const ssize_t chars_read = read (fd, filebuffer->ptr + filebuffer->len, buffer_len - filebuffer->len);

        if (chars_read == 0)
            break;
        else if (chars_read == -1)
        {
            fprintf (stderr, "%s: %s: %s\n", invocation_name, filename != 0 ? filename : "STDIN", strerror (errno));
            exit (2);
        }
        else
            filebuffer->len += chars_read;

        if (filebuffer->len >= buffer_len)
            filebuffer->ptr = xrealloc (filebuffer->ptr, buffer_len *= 2);
    }


    /* Close file.  */
    if (filename != 0 && close (fd) == -1)
    {
        fprintf (stderr, "%s: %s: %s\n", invocation_name, filename != 0 ? filename : "STDIN", strerror (errno));
        exit (2);
    }
}
