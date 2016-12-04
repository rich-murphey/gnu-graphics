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

/* This file is the draw_line routine, which is used by the other plot library
   routines. Coordinates are accumulated in the arrays line_x and line_y.
   The number of points accumulated is stored in PointsInLine. When draw_line
   is called it outputs any accumulated points.  PointsInLine is set to one
   with the first point replaced by the last point of the previous line - so
   that the next line can be a continuation of the previous one. */

#include "sys-defines.h"
#include "libplot.h"


/* plot (3) routines ___________________________________________ */

double x_input_min = 0.; /* minimum input x coordinate */
double y_input_min = 0.; /* minimum input y coordinate */
double x_output_min = 0.; /* minimum output x coordinate */
/* Center the plot on the page (11"-8.5")/2 72*dpi /.8 = 90 dots */
double y_output_min = 112.; /* minimum output y coordinate */
/* 8.5 in * 72dpi = 612 dots range: */
/* 8.5 in * 72dpi /.8 = 765 dots range: */
double x_output_max = 765.; /* maximum-minimum output x coordinate */
/* 90 min + 612 range +  = 702 dots max */
/* 112 min + 765 range +  = 877 dots max */
double y_output_max = 877.; /* maximum-minimum output y coordinate */
double scaleup = 10.; /* input to output scaleing for both x and y */

int last_x=0, last_y = 0; /* location of the last coordinates used */

/* the scaling and rotation part of a postscript transformation matrix */
double text_transformation_matrix[4] =
{
  1., 0., 0., 1.
};

/* Note: define MAX_NO_OF_POINTS according to the number of
   points the postscript device can handle.  Experimentation has
   shown 150 to be reasonable */

#define MAX_NO_OF_POINTS 150
int line_x[MAX_NO_OF_POINTS];
int line_y[MAX_NO_OF_POINTS];

/* We accumulate coordinates untill we either have enought points
   or the line is broken.  The counter tells us whether we have
   accumulated points and how many */

int PointsInLine=0;

/* this bit vector represents the line style (eg. dashed) for
   idraw.  We intitialize it to all ones which represents a solid
   line. */
long line_type_bit_vector = 65535;

/* this is a string that should conatain a postscript vector
   argument for the postscript setdash command.  This is allocted
   in the open(3) function. */
char *line_type_setdash;

/* the current length of the above buffer */
int line_type_setdash_length;

/* one greater than the length in number of bits in the dash pattern.  */

int line_type_setdash_bits=0;

/* LINE_WIDTH is the interger value of width of lines drawn. */

int line_width = 0;


/* draw a line using all the accumulated points if there are any. */

int
draw_line ()
{
  if (PointsInLine > 1 )
    {
      int i;
      fputs ("Begin %I MLine\n", stdout);
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
      printf("%%I %d\n", PointsInLine);
      for (i=0; i<PointsInLine; i++)
	{
	  printf ("%d %d\n", line_x[i], line_y[i]);
	  set_range (line_x[i]/scaleup, line_y[i]/scaleup);
	}
      printf ("%d MLine\nEnd\n\n", PointsInLine);

      /* leave the last point as the first point of the next line */
      line_x[0] = line_x [PointsInLine - 1];
      line_y[0] = line_y [PointsInLine - 1];
      PointsInLine = 1;
    }
  return 0;
}
