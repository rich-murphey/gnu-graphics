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

#include "sys-defines.h"
#include "libplot.h"

/* TEXT_ROTATION is the angle in degress counterclockwise from the
   horizontal for rotation of labels. */

int text_rotation=0;

/* FONT_SIZE is the font size in printer's points (-f option). */

double font_size = 14.;

/* ALABEL takes three arguments X_JUSTIFY, Y_JUSTIFY, and S and places
   the label S according to the x and y axis adjustments specified in
   X_JUSTIFY and Y_JUSTIFY respectively.  X_JUSTIFY is a character
   containing either l, c, or r for left, center or right justified with
   respect to the current x coordinate.  Y_JUSTIFY is a character
   containing either b, c, or t for placing the bottom center or top of
   the label even with the current y coordinate. S is a string containing
   the label. The current point is moved to follow the end of the text. */

int
alabel (x_justify, y_justify, s)
     char x_justify, y_justify, *s;
{
  fprintf (stdout, "T%c%c%s\n", x_justify, y_justify, s);
  return 0;
}
