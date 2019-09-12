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

/* This file is the space routine, which is a standard part of the plot
   library.  It sets the lower left and upper right corners of the page.  The
   plot will be scaled so that these coners fit the largest renderable area on
   the page.  */

#include "sys-defines.h"
#include "libplot.h"

int
space (x0, y0, x1, y1)
     int x0, y0, x1, y1;
{
  putchar ('s');
  putshort (x0);
  putshort (y0);
  putshort (x1);
  putshort (y1);
  return 0;
}
