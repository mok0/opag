/* This file provides a replacement for the strerror library function.
   Copyright (C) 2002 Martin Dickopp

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

#include <stdio.h>



char *
strerror (const int n)
{
    extern SYS_ERRLIST_TYPE sys_errlist [];
    extern int sys_nerr;

    if (n >= 0 && n < sys_nerr)
        return (char *)sys_errlist [n];

    {
        static char tmp [64];

        sprintf (tmp, "Unknown error %d", n);
        return tmp;
    }
}
