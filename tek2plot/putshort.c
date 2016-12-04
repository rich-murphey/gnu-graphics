/* Copyright (C) 1989 Free Software Foundation, Inc.

   plot is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY.  No author or distributor accepts responsibility to
   anyone for the consequences of using it or for whether it serves any
   particular purpose or works at all, unless he says so in writing.
   Refer to the GNU General Public License for full details.

   Everyone is granted permission to copy, modify and redistribute
   plot, but only under the conditions described in the GNU General
   Public License.  A copy of this license is supposed to have been
   given to you along with libtek so you can know your rights and
   responsibilities.  It should be in a file named COPYING.  Among
   other things, the copyright notice and this notice must be preserved
   on all copies.  */


int output_high_byte_first = 0;

/* This file is the putshort routine, which takes an integer argument
   and writes out the bottom two bytes, high byte first. */

#include "sys-defines.h"
#include "libplot.h"

void
putshort ( x)
     int x;
{
  if (output_high_byte_first == 1)
    {
      putchar ((x>>8) & 0xFF);
      putchar (x & 0xFF);
    }
  else if (output_high_byte_first == -1)
    {
      putchar (x & 0xFF);
      putchar ((x>>8) & 0xFF);
    }
  else
    {
      short i = x;
      fwrite (&i, sizeof(i), 1, stdout);
    }
}
