/* plot, unix plot to graphics device translators.
   Copyright (C) 1989 Free Software Foundation, Inc.

   plot is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY.  No author or distributor accepts responsibility to
   anyone for the consequences of using it or for whether it serves any
   particular purpose or works at all, unless he says so in writing.
   Refer to the GNU General Public License for full details.

   Everyone is granted permission to copy, modify and redistribute
   plot, but only under the conditions described in the GNU General
   Public License.  A copy of this license is supposed to have been given
   to you along with plot so you can know your rights and
   responsibilities.  It should be in a file named COPYING.  Among other
   things, the copyright notice and this notice must be preserved on all
   copies.  */

/* this file contains all the exernal references between plot library
   functions. The are not guaranteed to exist for a particular device
   and thus are not made available to the user in general. */

extern int font_id;
extern int line_style;
extern int last_x, last_y;
extern int PointsInLine;
extern double dash_length;
extern double text_rotation_angle;

extern double x_scale;      /* input to output scaleing for x */
extern double y_scale;      /* input to output scaleing for y */
