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
  double x_scale, y_scale;
#ifdef DEBUG
  printf ("%% space: %d %d %d %d\n", x0, y0, x1, y1);
#endif
  x_input_min = x0;
  y_input_min = y0;
  x_scale = (x1-x0)/(x_output_max-x_output_min);
  y_scale = (y1-y0)/(y_output_max-y_output_min);
  if (x_scale > y_scale)
    scaleup = x_scale;
  else
    scaleup = y_scale;
#ifdef DEBUG
  printf ("%% scale: %g %g %g %g %g %g\n",
	  x_scale, y_scale, x_input_min, y_input_min,
	  x_output_min, y_output_min);
#endif
  return 0;
}
