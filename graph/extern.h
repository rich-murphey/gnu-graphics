/* plot, unix plot file to graphics device translators.
   Copyright (C) 1989 Free Software Foundation, Inc.

   plot is distributed in the hope that it will be useful, but WITHOUT
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

#ifndef extern_h
#define extern_h
#include <stdio.h>

#ifdef __STDC__
#define P__(a)	a
#else
#define P__(a)	()
#define	const
#endif

#if	defined(__STDC__) || defined(HAVE_VOIDPTR)
#define	VOIDPTR	void
#else
#define	VOIDPTR	char
#endif

/* some machines do not define M_PI. */
#ifndef M_PI
#define M_PI 3.14159265358979323846264
#endif

typedef struct
{
  float x, y;  /* location of the point in user coordinates */
  char *string; /* a string containing the label for that point. */
  short symbol; /* this is either a number indicating which
		 symbol is to be plotted at the point or an
		 ascii character to be plotted depending
		 on the range of values:
		      0-31 a symbol number, or
		      32-127 an ascii character. */
  short linemode;
} point_struct;

VOIDPTR *
  do_malloc P__ ((int length));

VOIDPTR *
  do_realloc P__ ((VOIDPTR *p, int length));

extern void
  array_min P__ ((point_struct *p, int length, double *min_x, double *min_y));

extern void
  array_max P__ ((point_struct *p, int length, double *max_x, double *max_y));

/* this is the type of data we will read from the input files. */
typedef enum
{
 ASCII_DATA, INT_DATA, SHORT_INT_DATA, FLOAT_DATA, DOUBLE_DATA
 } data_type;

/* this is the type of grid and or tick marks */
typedef enum
{
  FULL_GRID, NO_GRID, TICKS_ALL_AROUND, TICKS_LEFT_AND_BELOW, AXES_AT_ORIGIN
  } grid_type;

extern int
  read_file P__(
    (FILE *in_stream,
     point_struct **p,
     int *length,
     int *no_of_points,
     int auto_abscissa,
     double x_start,
     double delta_x,
     data_type data_spec,
     int symbol_index,
     int transpose_axes,
     char *default_label,
     int linemode,		/* the line style for curves */
     int m_break_flag		/* non-zero means break when x decreases. */
     ));

int
  write_plot P__(
    (FILE *out_stream,
     point_struct *p,
     int length,
     double x_min,
     double x_max,
     double spacing_x,
     double y_min,
     double y_max,
     double spacing_y,
     double width,
     double height,
     double up,
     double right,
     double symbol_size,
     double tick_size,
     grid_type grid_spec,
     char *x_label,
     char *y_label,
     char *top_label,
     double margin,
     int log_axis,
     int break_flag,		/* non-zero means break lines after labels. */
     int round_to_next_tick,	/* round limits to the next tick mark */
     int default_fontsize,	/* size in printers points */
     int omit_labels		/* label both axes unless specified */
     ));


int
read_number P__((FILE *stream,
		   data_type data_spec,
		   float *value
		   ));

#undef P__

extern const char	*progname;	/* Program name */

#define	X_AXIS	1
#define Y_AXIS	2

#endif
