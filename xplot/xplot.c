/* This source file is based on the xplot written by Arthur Smith
<arthur@helios.tn.cornell.edu>.  It has been further modified by Rich
Murphey <Rich@rice.edu>.*/


#include "sys-defines.h"

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/Command.h>

#include "libplot.h"
#include "getopt.h"

FILE *ostrm = stderr;
int fork_flag = 1;		/* nonzero means fork before exit. */
#ifdef DEBUG
int debug_flag = 0;		/* nonzero means output debugging info. */
#endif
#ifndef DONT_HANDLE_SIGHUP
#include <signal.h>
int sighup_flag = 1;		/* nonzero means ignore sighup. */
#endif

char *b;			/* location of the bufffer */
int b_in=0;			/* where to read next char from */
int b_out=0;			/* where to write next char to */
int b_length=0;			/* number of bytes allocated */

char *sbuf;
int sbuf_length;

static XtCallbackProc Quit();
static XtCallbackProc do_plot ();
String fallback_resources[] = {
  "xplot*geometry:		500x500",
  "xplot*font:			fixed",
  NULL };

Display *dpy;
Pixmap pixmap;
GC gc;
XGCValues gcv;

int xmin=0, xmax=4096, ymin=0, ymax=4096;
double xfactor=1., yfactor=1.;
int last_x, last_y;

#define MAPX(x) ((int) (((x) - xmin) * xfactor))
#define MAPY(y) ((int) ((ymax - (y)) * yfactor))

int win_width=100, win_height=100;

Widget toplevel;
Widget child;
Arg wargs[20];
int nargc;
char **nargv;

char *startup_font = "";	/* null string means use sever default */
int high_byte_first = 0;
char *progname;			/* argv[0] or the name of this program */

void
display_version ()
{
    (void) fprintf (stderr, "\
%s version %s, by Arthur Smith <arthur@helios.tn.cornell.edu>, modified by\n\
Rich Murphey <Rich@rice.edu>.  %s comes with ABSOLUTELY NO WARRANTY.\n",
		    progname, VERS, progname);
}

/* Long options we recognize */
#define	ARG_NONE	0
#define	ARG_REQUIRED	1
#define	ARG_OPTIONAL	2

struct option	long_options[] = {
    { "fontname",	ARG_REQUIRED,	NULL, 'f' },
    { "help",		ARG_NONE,	NULL, 'H' },
    { "high-byte-first", ARG_NONE,	NULL, 'h' },
    { "low-byte-first",	ARG_NONE,	NULL, 'l' },
    { "no-fork",	ARG_NONE,	NULL, 'N' },
    { "version",	ARG_NONE,	NULL, 'V' },
    { "warranty",	ARG_NONE,	NULL, 'W' },
#ifndef DONT_HANDLE_SIGHUP
    { "catch-sighup",	ARG_NONE,	NULL, 'C' },
#endif
#ifdef DEBUG
    { "debug",		ARG_NONE,	NULL, 'D' },
#endif
    { NULL,		0,		NULL, 0}
};

void
display_help ()
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

int
main (argc, argv)
     int argc;
     char **argv;
{
  int i, bytes_recieved;
  int option;
  int opt_index;
  int errcnt = 0;		/* errors encountered */
  int show_version = 0;		/* remember to show version message */
  int show_usage = 0;		/* remember whether to output usage message. */
  int named_input = 0;		/* count named plot files on command line. */

  progname = argv[0];

  b_length = 128;		/* allocate the input buffer */
  b = (char*) malloc (b_length);

  while ((option = getopt_long (argc, argv, "CDHNVWf:g:hlx:", long_options, &opt_index)) != EOF) {
      if (option == 0)
	option = long_options[opt_index].val;

      switch (option)
	{
	case 'f':
	  startup_font = optarg;
	  break;
	case 'h':		/* read high byte first */
	  high_byte_first = 1;
	  break;
	case 'l':		/* read low byte first */
	  high_byte_first = -1;
	  break;
	case 'N':		/* do not fork */
	  fork_flag = 0;
	  break;
#ifdef DEBUG
	case 'D':		/* output debuggin info */
	  debug_flag = 1;
	  break;
#endif
#ifndef DONT_HANDLE_SIGHUP
	case 'C':
	  sighup_flag = 0;		/* do not ignore sighup. */
	  break;
#endif
	case 'H':		/* Help */
	  show_usage = 1;
	  break;
	case 'v':
	case 'V':		/* Version */
	  show_version = 1;
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
				    argv[0], optarg);
		    continue;
		  }
	      }

	    do {
#ifdef DEBUG
	      if (debug_flag) fprintf (ostrm, "input buffer: %d %d %d %d\n", b, b_in, b_out, b_length);
#endif
	      bytes_recieved = fread(&b[b_out], sizeof(char), b_length - b_out, data_file);
	      b_out += bytes_recieved > 0 ? bytes_recieved : 0;
	      if (b_out >= b_length)
		{ b_length *= 2;
		  b = (char*) realloc (b, b_length);}
	    } while (bytes_recieved > 0);
	    named_input++;

	    if (data_file != stdin) /* Don't close stdin */
	      fclose (data_file);
	  }
	  break;
	default:
	  errcnt++;
	  break;
	}
    }
  

#ifndef DONT_HANDLE_SIGHUP
  if (sighup_flag)
    signal(SIGHUP, SIG_IGN);
#endif

  if (!named_input)
    do {
      bytes_recieved = fread(&b[b_out], sizeof(char), b_length - b_out, stdin);
      b_out += bytes_recieved > 0 ? bytes_recieved : 0;
      if (b_out >= b_length)
	{ b_length *= 2;
	  b = (char*) realloc (b, b_length);}
    } while (bytes_recieved > 0);

  sbuf_length = 128;
  sbuf = (char*) malloc (sbuf_length);
#ifdef DEBUG
  if (debug_flag)
    {
      int i;
      fprintf (ostrm, "input buffer: %d %d %d %d\n", b, b_in, b_out, b_length);
      for (i=0; i<argc; i++)
	fprintf (ostrm, "args %d %s\n", i, argv[i]);
    }  
#endif
  nargc = argc;
  nargv = argv;

  if (b_out==0)
    fprintf (ostrm, "%s: Abort: zero bytes read from input.\n", argv[0]);
  else
    openpl ();
  return 0;
}
/* Openpl creates an X window which will contain the plot's pixmap. */
int openpl ()
{
  int i, narg;
  XtAppContext app_con;
  char *default_val;

  toplevel = XtAppInitialize(&app_con, "Xplot", NULL, ZERO, &nargc, nargv,
			     fallback_resources, NULL, ZERO);
  child = XtCreateManagedWidget("", commandWidgetClass, toplevel, NULL, ZERO);
  XtRealizeWidget (toplevel);
  dpy = XtDisplay(toplevel);
				/* get the size of the window */
  XtSetArg(wargs[0], XtNwidth, 0);
  XtSetArg(wargs[1], XtNheight, 0);
  XtGetValues(toplevel, wargs, TWO);
  win_width = wargs[0].value;
  win_height = wargs[1].value;
  if ((xmax + 1 - xmin)!=0)
    xfactor = ((double) win_width)/(xmax + 1 - xmin);
  else
    xfactor = win_width;
  if ((ymax + 1 - ymin)!=0)
    yfactor = ((double) win_height)/(ymax + 1 - ymin);
  else
    yfactor = win_height;

#ifdef DEBUG
  if (debug_flag) fprintf (ostrm, "pixmap: %d x %d\n", win_width, win_height);
#endif
				/* create the bitmap */
  pixmap = XCreatePixmap(dpy, DefaultRootWindow (dpy), win_width, win_height, 1);
  gc = XCreateGC (dpy, pixmap, 0L, &gcv);
				/* install the bitmap in the command widget */
  XtSetArg (wargs[0], XtNbitmap, pixmap);
  XtSetValues(child, wargs, ONE);
  XtAddCallback(child, XtNcallback, Quit, NULL);

  erase ();			/* erase the window */
  fontname (startup_font);	/* set the default font */

  do_plot (0, 0, 0);
  if (fork_flag)
    if (fork ())
      {
	sleep (1);		/* give the child window time to get mapped. */
	exit (0);
      }
  XtAppMainLoop(app_con);
}

static XtCallbackProc
do_plot (data, fd, id)
     XtPointer data;
     int *fd;
     XtInputId *id;
{
  char c;
  int x[6];
  while ((b_out - b_in) > 0)
    {
      c = b[b_in++];
#ifdef DEBUG
      if (debug_flag) fprintf (ostrm, "%c", c);
#endif
      switch (c)
	{
	case 'm':		/* Move */
	  if (getcoords (x, 2))
	    move (x[0], x[1]);
	  break;
	case 'n':		/* Continue */
	  if (getcoords (x, 2))
	    cont (x[0], x[1]);
	  break;
	case 'p':		/* Point */
	  if (getcoords (x, 2))
	    point (x[0], x[1]);
	  break;
	case 'l':		/* Draw Line */
	  if (getcoords (x, 4))
	    line (x[0], x[1], x[2], x[3]);
	  break;
	case 't':		/* Label */
	  if (getstring ())
	    alabel ('l', 'b', sbuf);
	  break;
	case 'T':		/* Adjusted Label */
	  {
	    char x_justify, y_justify;
	    x_justify = b[b_in++];
	    y_justify = b[b_in++];
#ifdef DEBUG
	    if (debug_flag) fprintf (ostrm, " %c %c", x_justify, y_justify);
#endif
	    if (getstring ())
	      alabel (x_justify, y_justify, sbuf);
	  }
	  break;
	case 'a':		/* Arc */
	  if (getcoords (x, 6))
	    arc (x[0], x[1], x[2], x[3], x[4], x[5]);
	  break;
	case 'c':		/* Circle */
	  if (getcoords (x, 3))
	    circle (x[0], x[1], x[2]);
	  break;
	case 'e':		/* Erase */
#ifdef DEBUG
	  if (debug_flag) fprintf (ostrm, "\n");
#endif
	  erase ();
	  break;
	case 'f':		/* Linemod */
	  if (getstring ())
	    linemod (sbuf);
	  break;
	case 's':		/* Space */
	  if (getcoords (x, 4))
	    space (x[0], x[1], x[2], x[3]);
	  break;
	case 'C':		/* Color */
	  if (getcoords (x, 3))
	    color (x[0], x[1], x[2]);
	  break;
	case 'F':		/* font name */
	  if (getstring ())
	    fontname (sbuf);
	  break;
	case 'S':		/* font size */
	  if (getcoords (x, 1))
	    fontsize (x[0]);
	  break;
	case 'r':		/* rotate */
	  if (getcoords (x, 3))
	    rotate (x[0], x[1], x[2]);
	  break;
	case 'L':		/* Fill */
	  if (getcoords (x, 1))
	    fill (x[0]);
	  break;
	default:
#ifdef DEBUG
	  fprintf (ostrm, "Unrecognized plot command `%c' (%o octal) ignored.\n", 
		  c, c&0xFF);
#endif
	  break;
	}
    }
  XClearArea (dpy, XtWindow (child), 0, 0, 0, 0, True);
}

drawellipse (x, y, r1, r2)
     int x, y, r1, r2;
{
  XDrawArc(dpy, pixmap, gc, x - r1, y - r2, 2 * r1, 2 * r2, 0, 64 * 360 - 1);
}

int
drawarc (x, y, x0, y0, x1, y1)
     int x, y, x0, y0, x1, y1;
{
  int a0, b0, a1, b1;
  int a02, b02, a12, b12;
  double ar, br, ar2, br2;
  double theta0, theta1;

  a0 = x0 - x;
  a02 = a0*a0;
  a1 = x1 - x;
  a12 = a1*a1;
  b0 = y0 - y;
  b02 = b0*b0;
  b1 = y1 - y;
  b12 = b1*b1;
  if (b12 == b02) return 0;	/* punt rather than divide by zero.  */
  ar2 = (a02*b12 - a12*b02)/(b12 - b02);
  if (ar2 < 0) return 0;
  ar = sqrt (ar2);
  if (a12 == a02) return 0;
  br2 = (b02*a12 - b12*a02)/(a12 - a02);
  if (br2 < 0) return 0;
  br = sqrt (br2);
  if ((br==0)||(ar==0)) return 0;
  theta0 = -atan2 (b0/br, a0/ar);
  theta1 = -atan2 (b1/br, a1/ar);
  if (theta0 > theta1) theta1 += 2 * M_PI;
  if (theta0 < 0)
    {
      theta0 += 2 * M_PI;
      theta1 += 2 * M_PI;
    }
  XDrawArc (dpy, pixmap, gc, (int) (x - ar), (int) (y - br), 
	    (int)(2 * ar), (int) (2 * br), 
	    (int) (64* theta0 * 180/M_PI), 
	    (int) (64 * (theta1 - theta0) * 180/M_PI));
  return 0;
}


/* Erase should wait for the current page to be absorbed, before creating
  a new one (presumably by erasing the pixmap and then redrawing the widget.*/
int
erase ()
{
				/* clear the screen */
  XSetFunction(dpy, gc, GXxor);
  XCopyArea(dpy, pixmap, pixmap, gc, 0, 0, win_width, win_height, 0, 0);

  XSetFunction(dpy, gc, GXcopyInverted);
  return 0;
}

int
label (s)
     char *s;
{
  XDrawString (dpy, pixmap, gc, MAPX (last_x), MAPY (last_y), s, strlen (s));
  return 0;
}

int
line (x1, y1, x2, y2)
     int x1, y1, x2, y2;
{
  XDrawLine (dpy, pixmap, gc, MAPX (x1), MAPY (y1), MAPX (x2), MAPY (y2));
  last_x = x2;
  last_y = y2;
  return 0;
}

int
circle (x, y, r)
     int x, y, r;
{
  drawellipse(MAPX(x), MAPY(y), (int) (r * xfactor), (int) (r*yfactor));
  last_x = x;
  last_y = y;
  return 0;
}


int
arc (x, y, x0, y0, x1, y1)
     int x, y, x0, y0, x1, y1;
{
  drawarc (MAPX (x), MAPY (y), MAPX (x0), MAPY (y0), MAPX (x1), MAPY (y1));
  last_x = x;
  last_y = y;
  return 0;
}

int
move (x, y)
     int x, y;
{
  last_x = x;
  last_y = y;
  return 0;
}

int
cont (x, y)
     int x, y;
{
  line (last_x, last_y, x, y);
  return 0;
}

int
point (x, y)
     int x, y;
{
  XDrawPoint (dpy, pixmap, gc, MAPX (x), MAPY (y));
  last_x = x;
  last_y = y;
  return 0;
}

int
linemod (s)
     char *s;
{
  int line_style;
  line_style = gcv.line_style;

  if (!strcmp (s, "solid"))
    line_style = LineSolid;
  else if ((!strcmp (s, "dotted"))||(!strcmp (s, "shortdashed")))
    line_style = LineOnOffDash;
  else if ((!strcmp (s, "longdashed"))||(!strcmp (s, "dotdashed")))
    line_style = LineDoubleDash;
  gcv.line_style = line_style;
  XChangeGC (dpy, gc, GCLineStyle, &gcv);
  return 0;
}

				/* swap the lower two bytes */
#define byte_swap(x) ((((x)&0xff)<<8) | (((x)<<8)&0xff00))

int
space (x0, y0, x1, y1)
     int x0, y0, x1, y1;
{
  xmin = x0;
  xmax = x1;
  ymin = y0;
  ymax = y1;
				/* determine the byte order */
  if ((high_byte_first == 0)
      && ((xmin > xmax)
	  || (ymin > ymax)
	  || (xmax < byte_swap(xmax))
	  || (ymax < byte_swap(ymax)) ))
    {
      xmin = byte_swap (xmin);
      xmax = byte_swap (xmax);
      ymin = byte_swap (ymin);
      ymax = byte_swap (ymax);
      high_byte_first = high_byte_first > 0 ? -1 : 1;
    }
#ifdef DEBUG
  if (debug_flag) fprintf (ostrm, "%s byte first -> %d %d %d %d\n",
			   (high_byte_first > 0 ? "high" : "low"),
			   xmin, ymin, xmax, ymax);
#endif
  if ((xmax + 1 - xmin)!=0)
    xfactor = ((double) win_width)/(xmax + 1 - xmin);
  else
    xfactor = win_width;
  if ((ymax + 1 - ymin)!=0)
    yfactor = ((double) win_height)/(ymax + 1 - ymin);
  else
    yfactor = win_height;
  return 0;
}

int
closepl ()
{
  XFreeGC (dpy, gc);
  XFreePixmap (dpy, pixmap);
  XtDestroyWidget (toplevel);
  return 0;
}

int getcoords (x, n)
     int *x;
     int n;
{
  unsigned char c, d;
  int i;

  if ((b_out - b_in) < 2*n) 
    return 0;
  for (i = 0; i < n; i++)
    {
      if (high_byte_first)
	{
	  d = b[b_in++];
	  c = b[b_in++];
	}
      else
	{
	  c = b[b_in++];
	  d = b[b_in++];
	}
      x[i] = (int) ((d << 8) | c);
#ifdef DEBUG
      if (debug_flag) fprintf (ostrm, " %d", x[i]);
#endif
    }
#ifdef DEBUG
  if (debug_flag) fprintf (ostrm, "\n");
#endif
  return 1;
}

int getstring ()
{
  char *string_end;
  int string_length;

  if ((string_end = index (&b[b_in], '\n')) == NULL)
    return 0;

  string_length = string_end - &b[b_in];
  while (string_length >= sbuf_length)
    { sbuf_length *= 2;
      sbuf = (char*) realloc (sbuf, sbuf_length);}
  bcopy (&b[b_in], sbuf, string_length);
  sbuf[string_length] = 0;
  b_in += string_length + 1;

#ifdef DEBUG
  if (debug_flag) fprintf (ostrm, " %s\n", sbuf);
#endif
  return string_length;
}

static XtCallbackProc 
Quit(widget, client_data, call_data)
     Widget	widget;		
     XtPointer client_data, call_data;
{
  XtDestroyApplicationContext(XtWidgetToApplicationContext(widget));
  exit(0);
}
