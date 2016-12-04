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

/* This file is the cont routine, which is a standard part of the plot
   library. It continues a line from the last point drawn to the point specified
   by x and y. */

#include "sys-defines.h"
#include "libplot.h"

int
cont (x, y)
     int x, y;
{
#ifdef DEBUG
  printf ("%% cont:%d %d\n", x, y);
#endif
  last_x = (x - x_input_min) + x_output_min;
  last_y = (y - y_input_min) + y_output_min;
  if (PointsInLine
      && (line_x[ PointsInLine-1] == last_x)
      && (line_y[ PointsInLine-1] == last_y))
    return 0;			/* slip redundant points */
  line_x[ PointsInLine] = last_x;
  line_y[ PointsInLine] = last_y;
  PointsInLine++;
  if (PointsInLine == MAX_NO_OF_POINTS)
    draw_line ();
  return 0;
}
