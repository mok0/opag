/* This file is part of opag, an option parser generator.
   Copyright (C) 2003 Martin Dickopp

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

#ifndef HDR_FILEREAD
#define HDR_FILEREAD 1


struct filebuffer {
    char *ptr;     /* Pointer to file buffer.  */
    size_t len;    /* Length of file buffer.  */
};


/* Read a file into memory and set `filebuffer' accordingly.  If `filename'
   is a null pointer, standard input is read.  */
extern void file_read (const char *filename, struct filebuffer *filebuffer) gcc_attr_nonnull ((2));


#endif
