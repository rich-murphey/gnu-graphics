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

/* This file is the cont routine, which is a standard part of the plot
   library. It continues a line from the last point drawn to the point
   specified by x and y.
   */
#include "sys-defines.h"
#include "libplot.h"
extern void putshort ();

double x_input_min = 0.; /* minimum input x coordinate */
double y_input_min = 0.; /* minimum input y coordinate */
double x_output_min = 0.; /* minimum output x coordinate */
double y_output_min = 0.; /* minimum output y coordinate */
double x_output_max = 767.; /* maximum-minimum output x coordinate */
double y_output_max = 767.; /* maximum-minimum output y coordinate */
double scaleup = 1.; /* default input to output scaleing for both x and y */

int last_x=0, last_y = 0; /* location of the last coordinates used */

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



int
cont (x, y)
     int x, y;
{
  putchar ('n');
  putshort (x);
  putshort (y);
  return 0;
}
