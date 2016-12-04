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

/* This file is the arc routine, which is an extension to the plot
   library.  It rotates subsequent labels to the given angle in degress
   from the horizontal */

#include "sys-defines.h"
#include "libplot.h"

int
rotate (w, h, angle)
     int w, h, angle;
{
  text_rotation = angle;
  if (angle == 0 )
    {
      text_transformation_matrix[0] = 1.;
      text_transformation_matrix[1] = 0.;
      text_transformation_matrix[2] = 0.;
      text_transformation_matrix[3] = 1.;
    }
  else
    {
      text_transformation_matrix[0] =       cos (M_PI * angle / 180. );
      text_transformation_matrix[1] =       sin (M_PI * angle / 180. );
      text_transformation_matrix[2] = -1. * sin (M_PI * angle / 180. );
      text_transformation_matrix[3] =       cos (M_PI * angle / 180. );
    }
  return 0;
}
