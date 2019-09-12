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
#include <ctype.h>
#include "getopt.h"
#include "libplot.h"
#include "extern.h"

#define	ARG_NONE	0
#define	ARG_REQUIRED	1
#define	ARG_OPTIONAL	2

struct option long_options[] =
{
  {"auto-abscissa",	ARG_OPTIONAL,	NULL, 'a'},
  {"bottom-move-margin", ARG_REQUIRED,	NULL, 'u'},
  {"break-non-monotone", ARG_NONE,	NULL, 'M'},
  {"break-on-labels",	ARG_NONE,	NULL, 'b'},
  {"data-format",	ARG_REQUIRED,	NULL, 'd'},
  {"dont-round-to-next-tick", ARG_NONE,	NULL, 'R'},
  {"fontsize",		ARG_REQUIRED,	NULL, 'f'},
  {"grid",		ARG_REQUIRED,	NULL, 'g'},
  {"height-plot",	ARG_REQUIRED,	NULL, 'h'},
  {"help",		ARG_NONE,	NULL, 'H'},
  {"linestyle",		ARG_REQUIRED,	NULL, 'm'},
  {"log-axis",		ARG_REQUIRED,	NULL, 'l'},
  {"no-label",		ARG_REQUIRED,	NULL, 'N'},
  {"point-label",	ARG_NONE,	NULL, 'c'},
  {"right-margin-posn",	ARG_REQUIRED,	NULL, 'r'},
  {"save-screen",	ARG_NONE,	NULL, 's'},
  {"symbol",		ARG_OPTIONAL,	NULL, 'S'},
  {"ticksize",		ARG_NONE,	NULL, 'T'},
  {"toptitle",		ARG_REQUIRED,	NULL, 'L'},
  {"transpose",		ARG_REQUIRED,	NULL, 't'},
  {"version",		ARG_NONE,	NULL, 'v'},
  {"width-plot",	ARG_REQUIRED,	NULL, 'w'},
  {"xlimits",		ARG_OPTIONAL,	NULL, 'x'},
  {"xtitle",		ARG_REQUIRED,	NULL, 'X'},
  {"ylimits",		ARG_OPTIONAL,	NULL, 'y'},
  {"ytitle",		ARG_REQUIRED,	NULL, 'Y'},
  /* Long options with no equivalent short option alias */
  {"debug",		ARG_NONE,	NULL, 'd' << 8},
  {"fontname",		ARG_REQUIRED,	NULL, 'f' << 8},
  {"margin",		ARG_REQUIRED,	NULL, 'm' << 8},
  {"copying",		ARG_NONE,	NULL, 'c' << 8},
  {"high-byte-first",	ARG_NONE,	NULL, 'H' << 8},
  {"low-byte-first",	ARG_NONE,	NULL, 'L' << 8},
  {NULL, 0, 0, 0}
};

const char *progname;		/* Program name */

VOIDPTR *
do_malloc (length)
     int length;
{
  VOIDPTR *p;
  p = (VOIDPTR *) malloc (length);

  if (p <= (VOIDPTR *) NULL)
    {
      perror ("malloc failed");
      exit (-1);
    }
  return p;
}


VOIDPTR *
do_realloc (p, length)
     VOIDPTR *p;
     int length;
{
  p = (VOIDPTR *) realloc (p, length);

  if (p <= (VOIDPTR *) NULL)
    {
      perror ("realloc failed");
      exit (-1);
    }
  return p;
}


void
usage ()
{
  int i;
  fprintf (stderr, "usage: %s", progname);
  for (i=0; long_options[i].name; i++)
    {
      fprintf (stderr, " [+%s", long_options[i].name);
      if (isprint (long_options[i].val))
	fprintf (stderr, "|-%c", long_options[i].val);
      if (long_options[i].has_arg == ARG_REQUIRED)
	fprintf (stderr, " arg]");
      else
	fprintf (stderr, "]");
    }
  fprintf (stderr, "\n");
}

void
display_version ()
{
  (void) fprintf (stderr, "\
graph version %s, Copyright (C) 1989 Free Software Foundation, Inc.\n\
graph comes with ABSOLUTELY NO WARRANTY; type `graph +warranty' for\n\
details.  This is free software, and you are welcome to redistribute it.\n\
Type `graph +copying' to view the copying conditions.\n\n", PACKAGE_VERSION);
}

int
main (argc, argv)
     int argc;
     char *argv[];
{
  int i;
  int option;
  int opt_index;
  int errcnt = 0;			/* errors encountered */
  int show_version = 0;		/* remember to show version message */
  int show_usage = 0;		/* remember whether to output usage message. */
  int show_copying = 0;		/* remember to show copying conditions */
  int opened = 0;		/* nonzero if we have opened named input file */
  int debug = 0;		/* debug option flag */
  int break_flag = 0;		/* non-zero means break lines after labels. */
  int m_break_flag = 0;		/* non-zero means break when x decreases. */
  int linemode_index = 0;	/* the line style for curves */
  int log_axis = 0;		/* no log axes by default */
  int omit_labels = 0;		/* label both axes by default */
  int transpose_axes = 0;	/* nonzero means -x applies to y axis. */
  data_type data_spec = ASCII_DATA; /* by default we read ascii data */
  grid_type grid_spec = FULL_GRID; /* by default we plot a full grid */
  int symbol_index = -1;	/* 0=none, 1=plus, 2=cross, 3=star */
  double symbol_size = .01;	/* fractional size of the symbols */
  double tick_size = .02;	/* fractional size of the tick marks */
  int default_fontsize = -1;	/* size in printers points for text.
				   -1 means use the default. */
  point_struct *p;		/* an array of points */
  int points_length;		/* the size of the points array */
  int no_of_points = 0;		/* the number of points in the array */
  int save_screen = 0;		/* save the screen == do not erase */
  int auto_abscissa = 0;	/* flag to turn on automatic generation
				   of abscissa */
  int round_to_next_tick = 1;	/* flag to specify rounding the limits
				   to the next tick mark */
  double x_start = 0.;		/* start and increment of auto abscissa */
  double delta_x = 1.;
  double margin_below = .1;	/* fractional margin below the plot */
  double margin_left = .1;	/* fractional margin left of the plot */
  double plot_height = .8;	/* fractional height of the plot */
  double plot_width = .8;	/* fractional width of the plot */
  char *point_label = 0;	/* label for each point, 0 means no label. */
  char *x_label = 0;		/* label for the x axis, 0 means no label.  */
  char *y_label = 0;		/* label for the y axis, 0 means no label.  */
  char *top_label = 0;		/* label above the plot, 0 means no label.  */
  double margin = 0.;		/* fractional margin between data and box */

  /* flags to indicate which limits the user has specified */
  int spec_min_x = 0, spec_min_y = 0, spec_max_x = 0, spec_max_y = 0;
  int spec_spacing_x = 0, spec_spacing_y = 0;
  /* user specified limits on the axes */
  double user_min_x = 0., user_min_y = 0., user_max_x = 0., user_max_y = 0.;
  double user_spacing_x = 0., user_spacing_y = 0.;
  double min_x, min_y, max_x, max_y;

  progname = strrchr (argv[0], '/');
  if (progname == NULL)
    progname = argv[0];
  else
    progname++;

  points_length = 4096;
  p = (point_struct *) do_malloc (points_length * sizeof (point_struct));

  errcnt = 0;

  while ((option = getopt_long (argc, argv, "-N:M::VbsvRL:T:c:X:Y:d:f:g:h:l:m:r:u:w:x::y::S::a::",
				long_options, &opt_index)) != EOF)
    {
      if (option == 0)
	option = long_options[opt_index].val;

      switch (option)
	{
	case 'a':		/* Auto-abscissa	*/
	  auto_abscissa = 1;
	  if (optind >= argc)
	    break;
	  if (sscanf (argv [optind], "%lf", &delta_x) <= 0)
	    break;
	  optind++;	/* tell getopt we recognized delta_x */
	  if (delta_x == 0.)
	    {
	      auto_abscissa = 0; /* reset abscissa option to the default */
	      delta_x = 1.;
	      x_start = 0.;
	    }
	  if (optind >= argc)
	    break;
	  if (sscanf (argv [optind], "%lf", &x_start) <= 0)
	    break;
	  optind++;	/* tell getopt we recognized x_start */
	  break;

	case 'b':		/* Break-on-labels	*/
	  break_flag = 1;
	  break;
	case 'M':		/* Break-non-monotone	*/
	  m_break_flag = 1;	/* same line style for each curve */
	  if (optind >= argc)	/* optind is the index of the optional arg */
	    break;
	  if (strcmp ("m", argv[optind]) == 0)
	    {
	      m_break_flag = 2;	/* switch line style when breaking lines */
	      optind++;		/* tell getopt we recognized the optional arg */
	    }
	  break;
	case 'f':		/* Fontsize		*/
	  if (isdigit(*optarg))
	    default_fontsize = atoi (optarg);
	  else
	    {
	      fprintf (stderr,
		       "%s: Error: fontsize must be an integer.  Was `%s'.\n",
		       progname, optarg);
	      errcnt++;
	    }
	  break;
	case 'g':		/* Grid-style		*/
	  switch (*optarg)
	    {
	    case '0':
	      grid_spec = NO_GRID;
	      break;
	    case '1':
	      grid_spec = FULL_GRID;
	      break;
	    case '2':
	      grid_spec = TICKS_ALL_AROUND;
	      break;
	    case '3':
	      grid_spec = TICKS_LEFT_AND_BELOW;
	      break;
	    case '4':
	      grid_spec = AXES_AT_ORIGIN;
	      break;
	    default:
	      fprintf (stderr,
		       "%s:  Unrecognized grid option `%s' ignored.\n",
		       progname, argv[i]);
	      errcnt++;
	    }
	  break;
	case 'd':		/* Data-format		*/
	  switch (*optarg)
	    {
	    case 'a':
	    case 'A':
	      data_spec = ASCII_DATA;
	      break;
	    case 'i':
	    case 'I':
	      data_spec = INT_DATA;
	      break;
	    case 's':
	    case 'S':
	      data_spec = SHORT_INT_DATA;
	      break;
	    case 'f':
	    case 'F':
	      data_spec = FLOAT_DATA;
	      break;
	    case 'd':
	    case 'D':
	      data_spec = DOUBLE_DATA;
	      break;
	    default:
	      fprintf (stderr,
		       "%s:  Unrecognized data option `%s' ignored.\n",
		       progname, argv[i]);
	      errcnt++;
	    }
	  break;
	case 'm':		/* Line-style		*/
	  if (isdigit(*optarg) || (*optarg == '-'))
	    linemode_index = atoi (optarg) - 1;
	  else
	    {
	      fprintf (stderr,
		       "%s: Error: line style must be an integer. Was `%s'.\n",
		       progname, optarg);
	      errcnt++;
	    }
	  break;
	case 'S':		/* Symbol		*/
	  symbol_index = 1;	/* symbol # 1 is the default! */

	  if (optind >= argc)
	    break;
	  if (sscanf (argv [optind], "%d", &symbol_index) <= 0)
	    break;
	  optind++;	/* tell getopt we recognized symbol_index */

	  if (optind >= argc)
	    break;
	  if (sscanf (argv [optind], "%lf", &symbol_size) <= 0)
	    break;
	  spec_min_x++;
	  optind++;	/* tell getopt we recognized symbol_size */
	  break;

	case 'H':		/* Help			*/
	  show_usage++;
	  show_version++;
	  break;
	case 'v':		/* Version		*/
	case 'V':
	  show_version++;
	  break;
	case 'T':		/* Ticksize		*/
	  tick_size = atof (optarg);
	  break;
	case 'L':		/* Top-title		*/
	  top_label = do_malloc (strlen (optarg) + 1);
	  strcpy (top_label, optarg);
	  break;
	case 'l':
	  switch (*optarg)
	    {
	    case 'x':
	    case 'X':
	      log_axis |= transpose_axes ? Y_AXIS : X_AXIS;
	      break;
	    case 'y':
	    case 'Y':
	      log_axis |= transpose_axes ? X_AXIS : Y_AXIS;
	      break;
	    default:
	      (void) fprintf (stderr, "%s:  ignoring unknown log axis type `%s'\n", progname, optarg);
	      break;
	    }
	  break;
	case 'N':		/* omit labels on the specified axis */
	  switch (*optarg)
	    {
	    case 'x':
	    case 'X':
	      omit_labels |= transpose_axes ? Y_AXIS : X_AXIS;
	      break;
	    case 'y':
	    case 'Y':
	      omit_labels |= transpose_axes ? X_AXIS : Y_AXIS;
	      break;
	    default:
	      (void) fprintf (stderr, "%s:  ignoring unrecognized axis specification `%s'\n", progname, optarg);
	      break;
	    }
	  switch (optarg[1])
	    {
	    case '\0':
	      break;
	    case 'x':
	    case 'X':
	      log_axis |= X_AXIS;
	      break;
	    case 'y':
	    case 'Y':
	      log_axis |= Y_AXIS;
	      break;
	    default:
	      (void) fprintf (stderr, "%s:  ignoring unknown log axis type `%s'\n", progname, optarg);
	      break;
	    }
	  break;
	case 'c':		/* point label */
	  point_label = do_malloc (strlen (optarg) + 1);
	  strcpy (point_label, optarg);
	  break;
	case 'X':		/* Xtitle		*/
	  if (transpose_axes) goto Ytitle;
	Xtitle:
	  x_label = do_malloc (strlen (optarg) + 1);
	  strcpy (x_label, optarg);
	  break;
	case 'Y':		/* Ytitle		*/
	  if (transpose_axes) goto Xtitle;
	Ytitle:
	  y_label = do_malloc (strlen (optarg) + 1);
	  strcpy (y_label, optarg);
	  break;
	case 'u':		/* bottom-margin-posn	*/
	  margin_below = atof (optarg);
	  break;
	case 'r':		/* right-margin-posn	*/
	  margin_left = atof (optarg);
	  break;
	case 'R':		/* round limits to the next tick mark */
	  round_to_next_tick = 0;
	  break;
	case 'w':		/* width-plot		*/
	  plot_width = atof (optarg);
	  break;
	case 'h':		/* height-plot		*/
	  plot_height = atof (optarg);
	  break;
	case 'x':		/* xlimits		*/
	  if (transpose_axes) goto ylimits;
	xlimits:
	  if (optind >= argc)	/* optind is the index of the optional arg */
	    break;
	  if (strcmp ("l", argv[optind]) == 0)
	    {
	      log_axis |= X_AXIS;
	      optind++;		/* tell getopt we recognized log axis arg */
	    }
	  if (optind >= argc)
	    break;
				/* parse minimum x argument */
	  if (sscanf (argv [optind], "%lf", &user_min_x) <= 0)
	    break;
	  spec_min_x++;
	  optind++;	/* tell getopt we recognized user_min_x */
	  if (optind >= argc)
	    break;
				/* parse maximum x argument */
	  if (sscanf (argv [optind], "%lf", &user_max_x) <= 0)
	    break;
	  spec_max_x++;
	  optind++;	/* tell getopt we recognized user_max_x */
	  if (optind >= argc)
	    break;
				/* parse x tick spacing argument */
	  if (sscanf (argv [optind], "%lf", &user_spacing_x) <= 0)
	    break;
	  spec_spacing_x++;
	  optind++;	/* tell getopt we recognized user_spacing_x */
	  break;

	case 'y':
	  if (transpose_axes) goto xlimits;
	ylimits:
	  if (optind >= argc)	/* optind is the index of the optional arg */
	    break;
	  if (strcmp ("l", argv[optind]) == 0)
	    {
	      log_axis |= Y_AXIS;
	      optind++;		/* tell getopt we recognized log axis arg */
	    }
	  if (optind >= argc)
	    break;
				/* parse minimum y argument */
	  if (sscanf (argv [optind], "%lf", &user_min_y) <= 0)
	    break;
	  spec_min_y++;
	  optind++;	/* tell getopt we recognized user_min_y */
	  if (optind >= argc)
	    break;
				/* parse maximum y argument */
	  if (sscanf (argv [optind], "%lf", &user_max_y) <= 0)
	    break;
	  spec_max_y++;
	  optind++;	/* tell getopt we recognized user_max_y */
	  if (optind >= argc)
	    break;
				/* parse y tick spacing argument */
	  if (sscanf (argv [optind], "%lf", &user_spacing_y) <= 0)
	    break;
	  spec_spacing_y++;
	  optind++;	/* tell getopt we recognized user_spacing_y */
	  break;

	case 's':
	  save_screen = 1;
	  break;
	case 't':
	  transpose_axes = 1;
	  break;
	  /*---------------- Long options below here ----------------*/
	case 'd' << 8:		/* debug */
	  debug = 1;
	  break;
	case 'f' << 8:		/* fontname */
	  (void) fontname (optarg);
	  break;
	case 'm' << 8:		/* margin */
	  margin = atof (optarg);
	  break;
	case 'w' << 8:		/* warranty */
	case 'c' << 8:		/* copying */
	  show_copying = 1;
	  break;
	case 'H' << 8:		/* output byte order */
	  output_high_byte_first = 1;
	  break;
	case 'L' << 8:		/* output byte order */
	  output_high_byte_first = -1;
	  break;
	default:
	  errcnt++;
	  break;
	case 1:
	  {
	    FILE *data_file;

	    if (strcmp (optarg, "-") == 0)
	      data_file = stdin;
	    else
	      {
		data_file = fopen (optarg, "r");
		if (data_file == NULL)
		  {
		    (void) fprintf (stderr, "%s:  ignoring nonexistent or inaccessible file `%s'\n",
				    progname, optarg);
		    continue;
		  }
	      }
	    opened++;
	    read_file (data_file, &p, &points_length, &no_of_points,
		       auto_abscissa, x_start, delta_x, data_spec,
		       symbol_index, transpose_axes, point_label,
		       linemode_index, m_break_flag);

	    if (data_file != stdin) /* Don't close stdin */
	      fclose (data_file);
	  }
	  break;
	}			/* endswitch */
    }				/* endwhile */


  if (show_version)
    display_version ();

  if ((show_usage > 0) || (errcnt > 0))
    usage ();

  if (errcnt > 0)
    exit (1);

  if (show_copying || show_usage)
    exit (0);

  /* Let's read in the named data files on the command line, if any.
     Read stdin if no files are given */

  if (optind < argc)
    {
      for (; optind < argc; optind++)
	{
	  FILE *data_file;

	  if (strcmp (argv[optind], "-") == 0)
	    data_file = stdin;
	  else
	    {
	      data_file = fopen (argv[optind], "r");
	      if (data_file == NULL)
		{
		  (void) fprintf (stderr, "%s:  ignoring nonexistent or inaccessible file `%s'\n",
				  progname, argv[optind]);
		  continue;
		}
	    }
	  opened++;
	  read_file (data_file, &p, &points_length, &no_of_points,
		     auto_abscissa, x_start, delta_x, data_spec, symbol_index,
		     transpose_axes, point_label, linemode_index, m_break_flag);

	  if (data_file != stdin) /* Don't close stdin */
	    fclose (data_file);
	}
    } /* endfor */

  if (!opened)
    /* Read stdin if no files were named on the command line. */
    read_file (stdin, &p, &points_length, &no_of_points, auto_abscissa,
	       x_start, delta_x, data_spec, symbol_index, transpose_axes,
	       point_label, linemode_index, m_break_flag);
  
  /* if the user doesn't specify all the limits, we compute them. */
  if (!spec_min_x || !spec_min_y)
    array_min (p, no_of_points, &min_x, &min_y);
  if (!spec_max_x || !spec_max_y)
    array_max (p, no_of_points, &max_x, &max_y);
  if (min_x == max_x)
    {
      min_x -= 1.;
      max_x += 1.;
    }
  if (min_y == max_y)
    {
      min_y -= 1.;
      max_y += 1.;
    }

  if (debug)
    {
      int j;

      fprintf (stderr, "%d elements read.\n", no_of_points);
      for (j = 0; j < no_of_points; j++)
	{
	  fprintf (stderr, "%g %g", p[j].x, p[j].y);
	  if (p[j].string)
	    fprintf (stderr, " \"%s\"", p[j].string);
	  if (p[j].symbol >= 0)
	    fprintf (stderr, " symbol %d", p[j].symbol);
	  if (p[j].linemode == 0)
	    putc ('\n', stderr);
	  else
	    fprintf (stderr, " linemode %d\n", p[j].linemode);
	}

      fprintf (stderr, "x min: %g max: %g\n", min_x, max_x);
      fprintf (stderr, "y min: %g max: %g\n", min_y, max_y);
      if (no_of_points == 0)
	fprintf (stderr, "Warning: no data read from input.\n");
      fflush (stderr);
    }
  if (!save_screen)
    erase ();

  if (   ((margin_below<=0.) && !x_label)
      || ((margin_left<=0.) && !y_label)
      || ((margin_below+plot_height>=1.) && !top_label))
    fprintf (stderr, "Warning: one of the margins is too narrow for a label.\n");

  /* If abscissal or ordinate values increase from left to right or
     bottom ot top respectively, spacing is positive, otherwise negative.
     Abort if the user specifies an inconsistent spacing. */
  if ( spec_spacing_x
      && ((user_max_x - user_min_x) * user_spacing_x <= 0.))
    {
      fprintf (stderr, "Error: x spacing must be the same sign as xmax-xmin.\n");
      exit (-1);
    }
  if ( spec_spacing_y
      && ((user_max_y - user_min_y) * user_spacing_y <= 0.))
    {
      fprintf (stderr, "Error: y spacing must be the same sign as ymax-ymin.\n");
      exit (-1);
    }


  write_plot (stdout, p, no_of_points,
	      spec_min_x ? user_min_x : min_x,
	      spec_max_x ? user_max_x : max_x,
	      spec_spacing_x ? user_spacing_x : 0.,
	      spec_min_y ? user_min_y : min_y,
	      spec_max_y ? user_max_y : max_y,
	      spec_spacing_y ? user_spacing_y : 0.,
	      plot_width, plot_height, margin_below, margin_left,
	      symbol_size, tick_size, grid_spec,
	      x_label, y_label, top_label, margin, log_axis,
	      break_flag, round_to_next_tick, default_fontsize,
	      omit_labels);
  return 0;
}
