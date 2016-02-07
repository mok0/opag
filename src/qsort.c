/* This file provides a replacement for the qsort library function.
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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include "stdinc.h"


/* Pointer to an element of the array.  */
#define ELEM(i) ((unsigned char *)array + size * (i))



/* Exchange two memory areas.  */
static inline void
exchange (unsigned char *a, unsigned char *b, size_t n)
{
    do
    {
        const unsigned char tmp = *a;
        *a++ = *b;
        *b++ = tmp;
    } while (--n > 0);
}



/* Rotate three memory areas.  */
static inline void
rotate (unsigned char *a, unsigned char *b, unsigned char *c, size_t n)
{
    do
    {
        const unsigned char tmp = *a;
        *a++ = *b;
        *b++ = *c;
        *c++ = tmp;
    } while (--n > 0);
}



void
qsort (void *array, size_t n, const size_t size, int (*const compare) (const void *, const void *))
{
    while (1)
    {
        switch (n)
        {
          case 0:
          case 1:
            return;

          case 2:
            if (compare (ELEM (0), ELEM (1)) > 0)
                exchange (ELEM (0), ELEM (1), size);
            return;

          case 3:
            if (compare (ELEM (0), ELEM (1)) > 0)
            {
                if (compare (ELEM (1), ELEM (2)) > 0)
                    exchange (ELEM (0), ELEM (2), size);
                else if (compare (ELEM (0), ELEM (2)) > 0)
                    rotate (ELEM (0), ELEM (1), ELEM (2), size);
                else
                    exchange (ELEM (0), ELEM (1), size);
            }
            else
            {
                if (compare (ELEM (0), ELEM (2)) > 0)
                    rotate (ELEM (2), ELEM (1), ELEM (0), size);
                else if (compare (ELEM (1), ELEM (2)) > 0)
                    exchange (ELEM (1), ELEM (2), size);
            }
            return;

          default:
            {
                size_t right = n - 1;

                while (compare (ELEM (right), ELEM (0)) > 0)
                    --right;

                if (right > 0)
                {
                    size_t pivot, left = 0;

                    exchange (ELEM (0), ELEM (right), size);
                    pivot = right--;

                    while (1)
                    {
                        while (compare (ELEM (right), ELEM (pivot)) > 0)
                            --right;

                        while (compare (ELEM (left), ELEM (pivot)) < 0)
                            ++left;

                        if (left < right)
                            exchange (ELEM (left++), ELEM (right--), size);
                        else
                            break;
                    }
                }

                if (++right > 1)
                    qsort (array, right, size, compare);

                if (right >= n - 1)
                    return;

                array = ELEM (right);
                n -= right;
            }
        }
    }
}
