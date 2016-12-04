/* This file is the line routine, which is a standard part of the plot
   library.  It draws a line from coordinate x0,y0 to x1,y1 */

#include "sys-defines.h"
#include "libplot.h"

int
line (x0, y0, x1, y1)
     int x0, y0, x1, y1;
{
  move (x0,y0);
  cont (x1,y1);
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
