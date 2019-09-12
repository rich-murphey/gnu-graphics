/* plot, a set of unix plot utilities.
   Copyright (C) 1989 Free Software Foundation, Inc.

   Plot is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY.  No author or distributor accepts responsibility to
   anyone for the consequences of using it or for whether it serves any
   particular purpose or works at all, unless he says so in writing.
   Refer to the GNU General Public License for full details.

   Everyone is granted permission to copy, modify and redistribute plot,
   but only under the conditions described in the GNU General Public
   License.  A copy of this license is supposed to have been given to you
   along with plot so you can know your rights and responsibilities.  It
   should be in a file named COPYING.  Among other things, the copyright
   notice and this notice must be preserved on all copies.  */

/* This file is the main routine for graph. */

#include "sys-defines.h"
#include <math.h>
#include "libplot.h"
#include "extern.h"

#define min(a,b) (a < b ? a : b)
#define max(a,b) (a > b ? a : b)

/*
 * Select step size to generate nice scale values for plotting.
 * The step size is always 1, 2, or 5.
 *
 * Reference:
 *	Lewart, C. R., "Algorithm 463:  Algorithms SCALE1, SCALE2, and
 *	SCALE3 for Determination of Scales on Computer Generated
 *	Plots[J6],"  Collected Algorithms from CACM.
 *
 */

double
scale1 (xmin, xmax, ntics)
     double xmin;		/* Data min		*/
     double xmax;		/* Data max		*/
     int ntics;			/* Approx tics wanted	*/
{
  int k;
  int nal;
  double a, b;

  /* The set of valid interval spacings */
  static double vint[] =
  {
    1.0, 2.0, 5.0, 10.0
  };

  /* Break points for selecting interval spacing */

  static double sqr[] =
  {
    1.414214, 3.162278, 7.071068
  };

  /* Find approximate interval size a */

  a = (xmax - xmin) / ntics;
  a *= xmax > xmin ? 1. : -1.;
  if (a <= 0.)
    a = 1.;			/* avoid log10 domain errors. */
  nal = (int) floor (log10 (a));

  /* Scale a between 1 and 10, and find the closest
     permissible value.  */

  b = a * pow (10.0, (double) -nal);
  for (k = 0; k < 3; k++)
    {
      if (b < sqr[k])
	break;
    }

  /* Compute interval size */

  return (xmax > xmin ? 1. : -1.) * vint[k] * pow (10.0, (double) nal);
}

/* log_limits expands the given plot limits to the next power of
   ten. */

void
log_limits (val_min, val_max, val_step, round)
     double *val_min;
     double *val_max;
     double *val_step;
     int round;
{
  *val_min = log10 (*val_min);
  *val_max = log10 (*val_max);
  if (round)
    {
      *val_min = ((*val_min < *val_max) ? floor : ceil) (*val_min);
      *val_max = ((*val_min < *val_max) ? ceil : floor) (*val_max);
    }
  *val_step = 1.;
}

/* write_plot takes as arguments x, y, and label arrrays containing the data
   and labels, and minimum and maximum x and y values. It scales the
   data and calls the appropriate plot routines. */

int
write_plot (out_stream, p, length,
	    x_min, x_max, x_spacing, y_min, y_max, y_spacing,
	    width, height, up, right,
	    symbol_size, tick_size, grid_spec,
	    x_label, y_label, top_label,
	    margin, log_axis,
	    break_flag, round_to_next_tick, default_fontsize,
	    omit_labels)
     FILE *out_stream;
     point_struct *p;
     int length;
     double x_min, x_max, x_spacing, y_min, y_max, y_spacing;
     double width, height, up, right;
     double symbol_size;
     double tick_size;
     grid_type grid_spec;
     char *x_label, *y_label, *top_label;
     double margin;
     int log_axis;
     int break_flag;		/* non-zero means break lines after labels. */
     int round_to_next_tick;	/* round limits to the next tick mark */
     int default_fontsize;	/* size in printers points */
     int omit_labels;		/* label both axes unless specified */
{
  /* there is no way you could use longer labels on tick marks! */
  char labelbuf[2048];
  char label_buf[2];		/* for single character labels */
  int index;			/* index into x, y, and label arrays */
  int need_break = 0;		/* non-zero after each label for breaks */
  int is_in_box = 0;		/* non-zero if a point is inside box limits*/
  int plot_width, plot_height, plot_up, plot_right, plot_range;
  double x_range, y_range;
  double xincr, yincr, xval, yval; /* tick spacing and value */
  double x_axis_y_val, y_axis_x_val;	/* position of each axis */
  int i, imax, j, xpos, ypos;	/* tick number, sign of tick increment */
  int linemode_index = -2;	/* only output line mode when it changes */

#define no_of_linemodes 6
#define lmod(x) (x % no_of_linemodes)

  static char *linemode_labels[] =
    {
      "solid",
      "dotted",
      "shortdashed",
      "dotdashed",
      "longdashed",
      "disconnected",
      };

#define plot_size 4096
  /* Check the data for valid log values, if we are using log axes */

  if ((log_axis & X_AXIS) && (x_min <= 0))
    {
      (void) fprintf (stderr,
		      "%s: error: You cannot have non-positive x values in a log plot\n",
		      progname);
      exit (1);
    }
  if ((log_axis & Y_AXIS) && (y_min <= 0))
    {
      (void) fprintf (stderr,
		      "%s: Error: You cannot have non-positive y values in a log plot\n",
		      progname);
      exit (1);
    }

  /* Preprocess the data if we have log axes.  In this case, the values
     we are working with now ALL refer to the log10 values of the data. */

  xpos = (x_min < x_max); /* non-zero if min is less than max */
  ypos = (y_min < y_max);

  if (log_axis & X_AXIS)
    {
      int k;
      x_range = x_max / x_min;
      x_min /= pow (x_range, margin);
      x_max *= pow (x_range, margin);

      for (k = 0; k < length; k++)
	p[k].x = log10 (p[k].x);
      log_limits (&x_min, &x_max, &xincr, round_to_next_tick);
    }
  else
    {
      x_range = x_max - x_min;
      x_min -= x_range * margin;
      x_max += x_range * margin;

      if (x_spacing == 0.)
	xincr = scale1 (x_min, x_max, 5);
      else
	xincr = x_spacing;
      if (round_to_next_tick)
	{
	  x_min = xincr * (xpos ? floor : ceil) (x_min / xincr);
	  x_max = xincr * (xpos ? ceil : floor) (x_max / xincr);
	}
    }
  x_range = x_max - x_min;


  if (log_axis & Y_AXIS)
    {
      int k;
      y_range = y_max / y_min;
      y_min /= pow (y_range, margin);
      y_max *= pow (y_range, margin);

      for (k = 0; k < length; k++)
	p[k].y = log10 (p[k].y);
      log_limits (&y_min, &y_max, &yincr, round_to_next_tick);
    }
  else
    {
      y_range = y_max - y_min;
      y_min -= y_range * margin;
      y_max += y_range * margin;

      if (y_spacing == 0.)
	yincr = scale1 (y_min, y_max, 5);
      else
	yincr = y_spacing;
      if (round_to_next_tick)
	{
	  y_min = yincr * (ypos ? floor : ceil) (y_min / yincr);
	  y_max = yincr * (ypos ? ceil : floor) (y_max / yincr);
	}
    }
  y_range = y_max - y_min;

  label_buf[1] = '\0';

  openpl ();
  space (0, 0, plot_size, plot_size);
  if (default_fontsize > 0)
    fontsize (default_fontsize);

  plot_up = (int)(up * plot_size);
  plot_right = (int)(right * plot_size);
  plot_width = (int)(width * plot_size);
  plot_height = (int)(height * plot_size);
  plot_range = max (plot_height, plot_width);

  /* we could optimize this by reducing the two transformations into one. */

  /* X Scale: convert from user x values to normalized corrdinates (0 to 1). */
#define XS(x) (((x) - x_min)/x_range)
  /* X Plot: convert from normalized x values to plot corrdinates. */
#define XP(x) ((int) (plot_right + plot_width * (x)))
  /* x value: normalize  and convert to plot coordinates. */
#define XV(x) XP(XS(x))
  /* Same as above, but for plotting log positions */
#define	LXV(x) XP(XS(log10(x)))

  /* Y Scale: convert from user y values to normalized corrdinates (0 to 1). */
#define YS(y) (((y) - y_min)/y_range)
  /* Y Plot: convert from normalized y values to plot corrdinates. */
#define YP(y) ((int) (plot_up + plot_height * (y)))
  /* y value: normalize and convert to plot coordinates. */
#define YV(y) YP(YS(y))
  /* Same as above, but for plotting log positions */
#define	LYV(y) YP(YS(log10(y)))

  /* Symbol Scale: convert from normalized coordinages to plot coordinates*/
#define SS(x) ((int) (plot_range * (x)))

  /* Draw the labels */
  if (top_label != NULL && *top_label != '\0')
    {
      /* tick_size < 0 specifies tics drawn outside the box */
      move (XP(.5), YP(1. + (tick_size >= 0. ? 1. : 2.) * fabs(tick_size)));
      alabel ('c', 'b', top_label);
    }
  if (grid_spec == AXES_AT_ORIGIN)
    {
      /* The position of each axis is at the value 0 (the origin) if
         the value 0 is between the limits of the opposing axis.
         Otherwise, the position is at the end closest to the value of
         0.  We precompute this value here, to make latter code more
         legible.  */
      y_axis_x_val = (x_min * x_max <= 0.) ? 0.
	: (x_min > 0. ? min (x_min, x_max) : max (x_min, x_max));
      x_axis_y_val = (y_min * y_max <= 0.) ? 0.
	: (y_min > 0. ? min (y_min, y_max) : max (y_min, y_max));
    }

  linemod ("solid");		/* make sure the axes are solid lines */
  if (x_label != NULL && *x_label != '\0')
    {
      double xmin, xmax;
      int no_of_tics;

      xmin = xincr * ((xpos ? ceil : floor) (x_min / xincr));
      xmax = xincr * ((xpos ? floor : ceil) (x_max / xincr));
      no_of_tics = (int) ((xmax - xmin) / xincr);
      
      if (grid_spec == AXES_AT_ORIGIN) 
	/* put the x label on the left end of the axis half way between
	   adjacent tick marks. */
	move (XV(x_min + xincr * .5), YV(x_axis_y_val)
	      - SS ((tick_size >= 0. ? 3. : 4.) * fabs(tick_size)));
      else
	/* put the x label in the middle of the axis half way between
	   adjacent tick marks. */
	move (XV(x_max - xincr * (.5 + (int)(no_of_tics / 2))),
	      YV (y_min) - SS ((tick_size >= 0. ? 3. : 4.) * fabs(tick_size)));
      alabel ('c', 't', x_label);
    }

  if (y_label != NULL && *y_label != '\0')
    {
      /* put the y label half way between adjacent tick marks in 
	 the middle of the axis. */
      double ymin, ymax;
      int no_of_tics;

      ymin = yincr * ((ypos ? ceil : floor) (y_min / yincr));
      ymax = yincr * ((ypos ? floor : ceil) (y_max / yincr));
      no_of_tics = (int) ((ymax - ymin) / yincr);

      if (grid_spec == AXES_AT_ORIGIN) 
	/* put the y label on the bottom of the axis half way between
	   adjacent tick marks. */
	move (XV(y_axis_x_val) - SS((tick_size >= 0. ? 2. : 3.) * fabs(tick_size)),
	      YV(y_min + yincr * .5));
      else
	/* put the y label in the middle of the axis half way between
	   adjacent tick marks. */
	move (XV(x_min) - SS((tick_size >= 0. ? 2. : 3.) * fabs(tick_size)),
	      YV(y_min + yincr * (.5 + (int)(no_of_tics / 2))));
      /* rotate (0, 0, 90); */
      alabel ('r', 'c', y_label);
      /* rotate (0, 0, 0); */
    }

  /* Draw a box around the plot.  If TICKS_LEFT_AND_BELOW is specified draw
     only the lower and right hand edges. If NO_GRID then omit the box. */

  switch (grid_spec)
    {
    default:
      move (XV(x_max), YV(y_min));
      cont (XV(x_max), YV(y_max));
      cont (XV(x_min), YV(y_max));
    case TICKS_LEFT_AND_BELOW:
      move (XV(x_min), YV(y_max));
      cont (XV(x_min), YV(y_min));
      move (XV(x_min), YV(y_min));
      cont (XV(x_max), YV(y_min));
      break;
    case AXES_AT_ORIGIN:
      move (XV(y_axis_x_val), YV(y_max));
      cont (XV(y_axis_x_val), YV(y_min));
      move (XV(x_min), YV(x_axis_y_val));
      cont (XV(x_max), YV(x_axis_y_val));
    case NO_GRID:;
    }

  /* We really need some exact arithmetic here so that we can get the
     precise value for each tick mark.  This avoids generating values like
     1e-15 or -0 as we approach 0. */
  
  i = (int) ((xpos ? ceil : floor) (x_min / xincr));
  imax = (int) ((xpos ? floor : ceil) (x_max / xincr));
  while (i <= imax)
    {
      xval = xincr * i;
      if (!(   (grid_spec == NO_GRID)
	    || (omit_labels & X_AXIS)
	    || (((xval==y_axis_x_val) || (i==0))
		&& (grid_spec == AXES_AT_ORIGIN)
		&& (ypos ? (y_min < 0.) : (y_min > 0.)))))
	{
	  move (XV (xval),
		YV ((grid_spec == AXES_AT_ORIGIN) ? x_axis_y_val : y_min)
		- SS ((tick_size >= 0. ? .5 : 1.5) * fabs(tick_size)));
	  sprintf (labelbuf, "%g", (log_axis & X_AXIS) ?
		   pow (10.0, xval) : xval);
	  alabel ('c', 't', labelbuf);
	}
      switch (grid_spec)
	{
	case TICKS_ALL_AROUND:
	  move (XV (xval), YV (y_max));
	  cont (XV (xval), YV (y_max) - SS (tick_size));
	case TICKS_LEFT_AND_BELOW:
	  move (XV (xval), YV (y_min));
	  cont (XV (xval), YV (y_min) + SS (tick_size));
	  break;
	  /* put FULL_GRID this into a seperate loop in versions 1.0+
	     so that we can issue the linemod only once for the whole
	     se of dahsed lines. */
	case FULL_GRID:
	  linemod ("shortdashed");
	  move (XV (xval), YV (y_min));
	  cont (XV (xval), YV (y_max));
	  linemod ("solid");
	  break;
	case NO_GRID:
	  break;
	case AXES_AT_ORIGIN:
	  move (XV (xval), YV (x_axis_y_val));
	  cont (XV (xval), YV (x_axis_y_val) + SS (tick_size));
	}
      i++;
    }

  i = (int) ((ypos ? ceil : floor) (y_min / yincr));
  imax = (int) ((ypos ? floor : ceil) (y_max / yincr));
  while (i <= imax)
    {
      yval = yincr * i;
      if (!(   (grid_spec == NO_GRID)
	    || (omit_labels & Y_AXIS)
	    || (((yval==x_axis_y_val) || (i==0))
		&& (grid_spec == AXES_AT_ORIGIN)
		&& (xpos ? (x_min < 0.) : (x_min > 0.)))))
	{
	  move (XV((grid_spec == AXES_AT_ORIGIN) ? y_axis_x_val : x_min)
		- SS((tick_size >= 0. ? .5 : 1.5) * fabs(tick_size)),
		YV (yval));
	  sprintf (labelbuf, "%g",
		   (log_axis & Y_AXIS) ? pow (10.0, yval) : yval);
	  alabel ('r', 'c', labelbuf);
	}
      switch (grid_spec)
	{
	case TICKS_ALL_AROUND:
	  move (XV (x_max), YV (yval));
	  cont (XV (x_max) - SS (tick_size), YV (yval));
	case TICKS_LEFT_AND_BELOW:
	  move (XV (x_min), YV (yval));
	  cont (XV (x_min) + SS (tick_size), YV (yval));
	  break;
	case FULL_GRID:
	  linemod ("shortdashed");
	  move (XV (x_min), YV (yval));
	  cont (XV (x_max), YV (yval));
	  linemod ("solid");
	  break;
	case NO_GRID:
	  break;
	case AXES_AT_ORIGIN:
	  move (XV (y_axis_x_val), YV (yval));
	  cont (XV (y_axis_x_val) + SS (tick_size), YV (yval));
	}
      i++;
    }

  /* Labels, ticks, and grids (if any) have been drawn.  We now want to
     draw some more tick marks if we have a log scale. */
  if ((log_axis & X_AXIS)
      && (grid_spec != NO_GRID)
      && (grid_spec != AXES_AT_ORIGIN))
    {
      double log_tick_size = tick_size * .5;

      for (i = (int) (xpos ? floor : ceil) (x_min / xincr);
	   xpos ? (i * xincr <= x_max) : (i * xincr >= x_max) ;
	   i += xpos ? 1 : -1)
	{
	  for (j = 1; j < 10; j++)
	    {
	      xval = pow (10., xincr * i) * j;

	      if ((pow(10., x_min) - xval) * (pow(10., x_max) - xval) <= 0.)
		{
		  switch (grid_spec)
		    {
		    case TICKS_ALL_AROUND:
		      move (LXV (xval), YV (y_max));
		      cont (LXV (xval), YV (y_max) - SS (log_tick_size));
		    case TICKS_LEFT_AND_BELOW:
		      move (LXV (xval), YV (y_min));
		      cont (LXV (xval), YV (y_min) + SS (log_tick_size));
		      break;
		    case FULL_GRID:
		      linemod ("shortdashed");
		      move (LXV (xval), YV (y_min));
		      cont (LXV (xval), YV (y_max));
		      linemod ("solid");
		      break;
		    case NO_GRID:
		    case AXES_AT_ORIGIN:
		      break;
		    }
		}
	    }
	}
    }

  if ((log_axis & Y_AXIS)
      && (grid_spec != NO_GRID)
      && (grid_spec != AXES_AT_ORIGIN))
    {
      double log_tick_size = tick_size * .5;

      for (i = (int) (ypos ? floor : ceil) (y_min / yincr);
	   ypos ? (i * yincr <= y_max) : (i * yincr >= y_max) ;
	   i += ypos ? 1 : -1)
	{
	  for (j = 1; j < 10; j++)
	    {
	      yval = pow (10., yincr * i) * j;

	      if ((pow(10., y_min) - yval) * (pow(10., y_max) - yval) <= 0.)
		{
		  switch (grid_spec)
		    {
		    case TICKS_ALL_AROUND:
		      move (XV (x_max), LYV (yval));
		      cont (XV (x_max) - SS (log_tick_size), LYV (yval));
		    case TICKS_LEFT_AND_BELOW:
		      move (XV (x_min), LYV (yval));
		      cont (XV (x_min) + SS (log_tick_size), LYV (yval));
		      break;
		    case FULL_GRID:
		      linemod ("shortdashed");
		      move (XV (x_min), LYV (yval));
		      cont (XV (x_max), LYV (yval));
		      linemod ("solid");
		      break;
		    case NO_GRID:
		    case AXES_AT_ORIGIN:
		      break;
		    }
		}
	    }
	}
    }


  for (index = 0; index < length; index++)
    {
      is_in_box = (((x_min - p[index].x) * (x_max - p[index].x)) <= 0.)
	&& (((y_min - p[index].y) * (y_max - p[index].y)) <= 0.);


      if (linemode_index != p[index].linemode)
	{
	  linemode_index = p[index].linemode;
	  if (linemode_index >= 0)
	    linemod (linemode_labels[lmod (linemode_index)]);
	}
      if (linemode_index < 0)
	move (XV (p[index].x), YV (p[index].y));
      else
	{
	  /* we should add clipping code here.  The following just
	     omits points outside the box.  It breaks lines where they
	     would go outside the box. */

	  if (! is_in_box)
	    {
	      need_break = 1;
	    }
	  else if (need_break)
	    {
	      move (XV (p[index].x), YV (p[index].y));
	      need_break = 0;
	    }
	  else
	    cont (XV (p[index].x), YV (p[index].y));
	}
      if (is_in_box && p[index].string)
	{
	  if (linemode_index < 0)
	    move (XV (p[index].x), YV (p[index].y));
	  alabel ('l', 'c', p[index].string);
	  move (XV (p[index].x), YV (p[index].y));
	  need_break |= break_flag;
	}
      if (is_in_box && p[index].symbol >= 0)
	{
	  if (p[index].symbol > 31)
	    {
	      /* copy the character symbol into a string and plot it
		 as an adjusted label. */
	      label_buf[0] = (char) p[index].symbol;
	      alabel ('c', 'c', label_buf);
	    }
	  else
	    {
	      linemod ("solid");
	      switch (p[index].symbol)
		{
		case 0:		/* plus */
		  move (XV (p[index].x) - SS (symbol_size), YV (p[index].y));
		  cont (XV (p[index].x) + SS (symbol_size), YV (p[index].y));
		  move (XV (p[index].x), YV (p[index].y) - SS (symbol_size));
		  cont (XV (p[index].x), YV (p[index].y) + SS (symbol_size));
		  break;
		case 1:		/* cross */
		  move (XV (p[index].x) - SS (symbol_size),
			YV (p[index].y) - SS (symbol_size));
		  cont (XV (p[index].x) + SS (symbol_size),
			YV (p[index].y) + SS (symbol_size));
		  move (XV (p[index].x) + SS (symbol_size),
			YV (p[index].y) - SS (symbol_size));
		  cont (XV (p[index].x) - SS (symbol_size),
			YV (p[index].y) + SS (symbol_size));
		  break;
		case 2:		/* star */
		  move (XV (p[index].x) - SS (symbol_size), YV (p[index].y));
		  cont (XV (p[index].x) + SS (symbol_size), YV (p[index].y));
		  move (XV (p[index].x) - SS (symbol_size * .5),
			YV (p[index].y) + SS (symbol_size * .866));
		  cont (XV (p[index].x) + SS (symbol_size * .5),
			YV (p[index].y) - SS (symbol_size * .866));
		  move (XV (p[index].x) + SS (symbol_size * .5),
			YV (p[index].y) + SS (symbol_size * .866));
		  cont (XV (p[index].x) - SS (symbol_size * .5),
			YV (p[index].y) - SS (symbol_size * .866));
		  break;
		case 3:		/* box */
		  move (XV (p[index].x) - SS (symbol_size),
			YV (p[index].y) - SS (symbol_size));
		  cont (XV (p[index].x) + SS (symbol_size),
			YV (p[index].y) - SS (symbol_size));
		  cont (XV (p[index].x) + SS (symbol_size),
			YV (p[index].y) + SS (symbol_size));
		  cont (XV (p[index].x) - SS (symbol_size),
			YV (p[index].y) + SS (symbol_size));
		  cont (XV (p[index].x) - SS (symbol_size),
			YV (p[index].y) - SS (symbol_size));
		  break;
		case 4:		/* diamond */
		  move (XV (p[index].x),
			YV (p[index].y) - SS (symbol_size));
		  cont (XV (p[index].x) + SS (symbol_size),
			YV (p[index].y));
		  cont (XV (p[index].x),
			YV (p[index].y) + SS (symbol_size));
		  cont (XV (p[index].x) - SS (symbol_size),
			YV (p[index].y));
		  cont (XV (p[index].x),
			YV (p[index].y) - SS (symbol_size));
		  break;
		case 5:		/* circle */
		  circle (XV (p[index].x), YV (p[index].y), SS (symbol_size));
		  break;
		}
	      if (linemode_index >= 0)
		linemod (linemode_labels[lmod (linemode_index)]);
	      move (XV (p[index].x), YV (p[index].y));
	    }
	}
    }
  closepl ();
  return 0;
}
