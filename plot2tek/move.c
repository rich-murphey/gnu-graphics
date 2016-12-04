/* This file is the move routine, which is a standard part of the plot
   library.  It switches the device to vector plot mode and plots a
   point at the given coordinate.
*/
#include "sys-defines.h"
#include "libplot.h"

int
move (x, y)
     int x, y;
{
  putchar ('\035');	/* ASCII GS, vector plot mode */
  cont (x, y);
  return 0;
}
/* libtek, a library of functions for tektronics 4010 compatible devices.
   Copyright (C) 1989 Free Software Foundation, Inc.

libtek is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY.  No author or distributor accepts responsibility to anyone for the
consequences of using it or for whether it serves any particular purpose or
works at all, unless he says so in writing.  Refer to the GNU General Public
License for full details.

Everyone is granted permission to copy, modify and redistribute libtek, but
only under the conditions described in the GNU General Public License.  A copy
of this license is supposed to have been given to you along with libtek so
you can know your rights and responsibilities.  It should be in a file named
COPYING.  Among other things, the copyright notice and this notice must be
preserved on all copies.  */
