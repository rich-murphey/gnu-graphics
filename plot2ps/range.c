/* plot2ps, a utility for converting Unix plot files into postscript.
   Copyright (C) 1989 Free Software Foundation, Inc.

Plot2ps is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY.  No author or distributor accepts responsibility to anyone for the
consequences of using it or for whether it serves any particular purpose or
works at all, unless he says so in writing.  Refer to the GNU General Public
License for full details.

Everyone is granted permission to copy, modify and redistribute plot2ps, but
only under the conditions described in the GNU General Public License.  A copy
of this license is supposed to have been given to you along with plot2ps so
you can know your rights and responsibilities.  It should be in a file named
COPYING.  Among other things, the copyright notice and this notice must be
preserved on all copies.  */

/* This file contains the get_range and set_range routines which set and
   return the range of output corrdinates in printers points suitable for
   use in an EPS boundingbox comment. */

#include "sys-defines.h"
#include "libplot.h"
double x_max = -9e9, x_min = 9e9, y_max = -9e9, y_min = 9e9;

void set_range (x, y)
     double x, y;
{
  if (x > x_max) x_max = x;
  if (x < x_min) x_min = x;
  if (y > y_max) y_max = y;
  if (y < y_min) y_min = y;
}

void get_range (xmin, xmax, ymin, ymax)
     double *xmin, *xmax, *ymin, *ymax;
{
  *xmax = x_max;
  *xmin = x_min;
  *ymax = y_max;
  *ymin = y_min;
}
