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

/* This file is the arc routine, which is a standard part of the plot
   library.  It draws an arc with the center at xc,yc, the beginning at
   x0,y0 and the ending at x1,y1 */

#include "sys-defines.h"
#include "libplot.h"

#ifdef NEED_DREM
/* return the remainder of x/y */
double drem (x, y)
     double x, y;
{
  while (x>y/2.)
    x -= y;
  while (x<-y/2.)
    x += y;
  return x;
}
#endif

int
arc (xc, yc, x0, y0, x1, y1)
     int xc, yc, x0, y0, x1, y1;
{
  /* center: xc,yc
     from: x0,y0
     to: x1,y1 */
  double dtheta, theta, theta_0, theta_1, x, y, radius;

  x = x0 - xc;			/* starting coordinate relatve to center */
  y = y0 - yc;			/* of the circle. */
  radius = sqrt (x*x + y*y);

  if (x != 0)			/* find the starting angle */
    {
      theta_0 = atan2 (y, x);
    }
  else
    {
      if (y>0)
	theta_0 = M_PI/2.;
      else
	theta_0 = M_PI/-2.;
    }
      
  x = x1 - xc;			/* ending coordinate relatve to center */
  y = y1 - yc;			/* of the circle. */

  if (x != 0)			/* find the ending angle */
    {
      theta_1 = atan2 (y, x);
    }
  else
    {
      if (y>0)
	theta_1 = M_PI/2.;
      else
	theta_1 = M_PI/-2.;
    }
  if (theta_1 <= theta_0)
    theta_1 += 2. * M_PI;

#ifdef HAVE_ARC_OP
  theta_0 *= 180. / M_PI;
  theta_1 *= 180. / M_PI;

  fprintf (stdout, "\033/%d;%d;%d;%d;%dA", 
	   xc, yc, (int) radius, (int) theta_0,
	   (int) (theta_1 - theta_0));

#else /* HAVE_ARC_OP */
  move (x0, y0);

  /*  we make dtheta proportional to the lenth of the arc so that
      small arcs will look smooth. */
  dtheta = (theta_1 - theta_0) /64.;

  for (theta = theta_0 + dtheta; theta < theta_1; theta += dtheta)
    {
      double xx, yy, xinc, yinc, distance_a, distance_b;
      xx = radius*cos(theta);
      yy = radius*sin(theta);

      /* choose the nearest pixel. */
      if ( drem (theta, M_PI/2.) < M_PI/-4.)
	{
	  xinc = 1.;
	  yinc = 0.;
	}
      else
	{
	  xinc = 0.;
	  yinc = 1.;
	}
      distance_a = (int)(xx) * (int)(xx)
	+ (int)(yy) * (int)(yy) - radius * radius;
      distance_b = (int)(xx+xinc) * (int)(xx+xinc)
	+ (int)(yy+yinc) * (int)(yy+yinc) - radius * radius;
      if (distance_b < distance_a)
	{
	  xx += xinc;
	  yy += yinc;
	}
      else
	{
	  distance_b = (int)(xx-xinc) * (int)(xx-xinc)
	    + (int)(yy-yinc) * (int)(yy-yinc) - radius * radius;
	  if (distance_b < distance_a)
	    {
	      xx -= xinc;
	      yy -= yinc;
	    }
	}
      cont ((int) (xc + xx), (int) (yc + yy));
    }
  cont (x1, y1);
  move (xc, yc);
#endif /* HAVE_ARC_OP */
  return 0;
}
