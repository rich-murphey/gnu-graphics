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
     int x_justify, y_justify;
     char *s;
{
  int xposn, yposn;
  double width, theta;
  double x_char_offset = 0., x_extra = 0., y_char_offset = 0.;

  width = strlen (s);
  switch (x_justify)
    {
    case 'l': /* left justified */
      x_char_offset = 0.0;
      break;

    case 'c': /* centered */
      x_char_offset = 0.5;
      break;

    case 'r': /* right justified */
      x_char_offset = 1.;
      x_extra = .4;
      break;
    }

  switch (y_justify)
    {
    case 'b': /* bottom */
      y_char_offset = 0.;
      break;

    case 'c': /* centered */
      y_char_offset = .6;
      break;

    case 't': /* top */
      y_char_offset = 1.5;
      break;
    }

  /* Position the text according to the justification and text
     rotation.  If necessary, the cursor is moved to a new
     location where the string would begin. */
  
  theta = text_rotation * M_PI / 180;
  
  xposn = last_x;
  yposn = last_y;
  if ( (x_char_offset != 0.)
    || (y_char_offset != 0.))
    {
      xposn = last_x - (int) (font_size * x_char_offset
			    * (width + x_extra) * scaleup * cos(theta));
      yposn = last_y - (int) (font_size * y_char_offset * scaleup * cos(theta));
      move (xposn, yposn);
    }


  if (text_rotation == 0) {
      /* No rotation, so do it the easy way */
      putc ('\037', stdout);
      fputs (s, stdout);
  } else if (text_rotation == 90) {
      /* This is a special case.  If text rotation is 90 (for
	 labels on the y-axis), plot the label down, instead
	 of up as would normally be done. */
      char *ptr;
      int cnt;

      cnt = strlen(s);
      ptr = s + cnt;
      while (cnt-- > 0) {
	  putchar ('\037');
	  putchar (*--ptr);
	  yposn += 1.3 * font_size * scaleup;
	  move (xposn, yposn);
      }
  } else {
      /* General case:  move the cursor and plot each
	 character individually */
      
      int dx, dy;

      dx = 0.5 + font_size * scaleup * cos(theta);
      dy = 0.5 + font_size * scaleup * sin(theta);

      while (*s) {
	  putchar ('\037');
	  putchar (*s);
	  xposn += dx;
	  yposn += dy;
	  move (xposn, yposn);
	  s++;
      }
  }

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
