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

/* This file is the circle routine, which is a standard part of the plot
   library. It draws a circle given the center x,y and the radius */

#include "sys-defines.h"
#include "libplot.h"

int
circle (x, y, r)
     int x, y, r;
{
  int radius;
  draw_line ();
  fputs ("Begin %I Elli\n", stdout);
  printf("%%I b %d\n", line_type_bit_vector);
  printf ("%d 0 0 [ %s ] %d SetB\n", line_width, line_type_setdash,
	  line_type_setdash_bits);
  printf ("%%I cfg Black\n%g %g %g SetCFg\n",
	  fgcolor_red, fgcolor_green, fgcolor_blue);
  fputs ("%I cbg White\n1 1 1 SetCBg\n", stdout);
  if (fill_level == -1.)
    {
      printf ("%%I p\nnone SetP\n");
    }
  else
    {
      printf ("%%I p\n%g SetP\n", fill_level);
    }
  printf ("%%I t\n[ %g 0 0 %g %g %g ] concat\n", 1./scaleup, 1./scaleup,
	  x_output_min, y_output_min);
  last_x = (x - x_input_min) + x_output_min;
  last_y = (y - y_input_min) + y_output_min;
  radius = r;
  printf ("%%I\n%d %d %d %d Elli\nEnd\n\n", last_x, last_y, radius, radius);
#ifdef DEBUG
  printf ("%% circle: %d %d %d\n", x, y, r);
#endif
  return 0;
}
