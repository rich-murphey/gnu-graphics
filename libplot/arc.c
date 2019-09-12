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

/* This file is the arc routine, which is a standard part of the plot
   library.  It draws an arc with the center at xc,yc, the beginning at
   x0,y0 and the ending at x1,y1 */

#include "sys-defines.h"
#include "libplot.h"

int
arc (xc, yc, x0, y0, x1, y1)
     int xc, yc, x0, y0, x1, y1;
{
  putchar ('a');
  putshort (xc);
  putshort (yc);
  putshort (x0);
  putshort (y0);
  putshort (x1);
  putshort (y1);
  return 0;
}
