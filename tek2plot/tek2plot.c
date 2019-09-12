/* This program, plot, translates a GNU plot file into device specific output.
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

/* This file is the main routine for plot.

   It includes code to read the plot file and call plot functions
   to draw the graphics. */

#include "sys-defines.h"
#include "libplot.h"
#include "getopt.h"

int read_byte (stream)
     FILE *stream;
{
  int i;
  i = (unsigned) getc (stream);
  /* printf ("<%o %o %o %c>", i>>5, i&0x1F, i, i);*/
  return i;
}

/* read_plot reads a plot file from the standard input and calls
   a plot function according to each plot instruction found in the
   file. */

void
read_plot (in_stream, buffer, buffer_length)
     FILE *in_stream;
     char *buffer;
     int  buffer_length;
{
  int x, y, text_x, text_y, dots_on=1, continued=0;
  int byte_read;

  byte_read = read_byte (in_stream);
  while (!feof (in_stream))
    {
      if (   ((byte_read>>5) == 1)
	  || ((byte_read>>5) == 2)
	  || ((byte_read>>5) == 3))
	{
	  if ((byte_read>>5) == 1) /* high 5 bits of y */
	    {
	      y = (y & 0x1F) | ((byte_read & 0x1F)<<5);
	      byte_read = read_byte (in_stream);
	    }
	  if ((byte_read>>5) == 3) /* low 5 bits of y */
	    {
	      while ((byte_read>>5) == 3)
		{
		  y = (y & (0x1F<<5)) | (byte_read & 0x1F);
		  byte_read = read_byte (in_stream);
		}
	      /* only read high part of x if there was a low part of y */
	      if ((byte_read>>5) == 1) /* high 5 bits of x */
		{
		  x = (x & 0x1F) | ((byte_read & 0x1F)<<5);
		  byte_read = read_byte (in_stream);
		}
	      /* low 5 bits of y */
	      x = (x & (0x1F<<5)) | (byte_read & 0x1F);
	    }
	  text_x = x;
	  text_y = y;
	  if (continued)
	    {
	      cont (x, y);
	    }
	  else
	    {
	      move (x, y);
	      continued = 1;
	    }
	}
      else
	switch (byte_read)
	  {
	  case '\000':		/* ignore nulls */
	    break;
	  case '\007':		/* ctrl G - BELL */
	    break;
	  case '\010':		/* ctrl H - backspace */
	    text_x -= 10;
	    break;
	  case '\011':		/* ctrl I - tab */
	    text_x += 10;
	    break;
	  case '\012':		/* ctrl J - linefeed */
	    text_y -= 17;
	    break;
	  case '\013':		/* ctrl K - vertical tab */
	    text_y += 17;
	    break;
	  case '\030':		/* alphanumerics mode */
	    break;
	  case '\015':		/* ctrl M - alphagraphics mode with reset */
	    text_y -= 10;
	    text_x = 0;
	    dots_on = 1;
	  case '\037':		/* alphagraphics mode */
	    {
	      char *p;
	      p = buffer;
	      *p = read_byte (in_stream);
	      while (*p > 31)
		{
		  *(++p) = read_byte (in_stream);
		}
	      ungetc (*p, in_stream);
	      if (p != buffer)
		{
		  *p = 0;
		  move (text_x, text_y);
		  label (buffer);
		  text_x += strlen(buffer) * 12;
		  continued = 0;
		}
	    }
	    break;
	  case '\033':          /* ESC */
	    {
	      byte_read = read_byte (in_stream);
	      switch (byte_read)
		{
		case '\014':	/* ctrl l */
		  erase ();
		  fontsize (8);
		  linemod ("solid");
		  dots_on = 1;
		  x = 0;
		  y = 0;
		  break;
		case '0':
		  fontsize (8);
		  break;
		case '1':
		  fontsize (10);
		  break;
		case '2':
		  fontsize (12);
		  break;
		case '3':
		  fontsize (14);
		  break;
		case '@':
		  fill (1);
		  break;
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		  fill (0x7FFF);
		  break;
		case '`':
		  linemod ("solid");
		  break;
		case 'a':
		  linemod ("dotted");
		  break;
		case 'b':
		  linemod ("dotdashed");
		  break;
		case 'c':
		  linemod ("shortdashed");
		  break;
		case 'd':
		  linemod ("longdashed\n");
		  break;
		case 'x':
		  linemod ("user1");
		  break;
		case 'y':
		  linemod ("user2");
		  break;
		case 'z':
		  linemod ("user3");
		  break;
		case '/':
		  {
		    int i=0, x[10];
		    sscanf("%d", (char*) &x[i]);
		    byte_read = read_byte (in_stream);
		    while (byte_read == ';')
		      {
			i++;
			sscanf("%d", (char*) &x[i]);
			byte_read = read_byte (in_stream);
		      }
		    switch (byte_read)
		      {
		      case 'e':
			x[0] = (x[0]%8)*45 - 90;
			if (x[0] < 0)
			  x[0] += 360;
			rotate (0, 0, x[0]);
			break;
		      case 'd':
			switch (x[0])
			  {
			  case 0:
			    dots_on = 1;
			    break;
			  case 1:
			    dots_on = 0;
			    break;
			  }
			break;
		      case 'A': /* arc */
			{
			  if (i < 3)
			    x[3] = 0;
			  if (i < 4)
			    x[4] = 360;
			  arc (x[0], x[1],
			       (int) (x[0] + x[2] * cos (x[3]*M_PI/180.)),
			       (int) (x[1] + x[2] * sin (x[3]*M_PI/180.)),
			       (int) (x[0] + x[2] * cos ((x[3]+x[4])*M_PI/180.)),
			       (int) (x[1] + x[2] * sin ((x[3]+x[4])*M_PI/180.)));
			}
			break;
		      }
		  }
		  break;
		case '[':
		  {
                    if ((read_byte (in_stream) == '?') &&
                        (read_byte (in_stream) == '3') &&
                        (read_byte (in_stream) == '8') &&
                        (read_byte (in_stream) == 'h'))
                    {
                      break;
                    } else {
                      fprintf (stderr, "Unrecognized escape code ignored.\n");
                    }
                  }
		}
	    }
	    break;
	  case '\034':		/* point plot mode */
	    /* printf("point plot mode.\n");*/
	    break;
	  case '\035':		/* vector mode */
	    /* printf("vector mode.\n");*/
	    continued = 0;
	    break;
	  case '\036':		/* incremental point plot mode */
	    {
	      int x_incr = 0, y_incr = 0;
	      /* printf("incremental point plot: <");*/
	      while(!feof (in_stream) && (byte_read != '\037'))
		{
		  byte_read = read_byte (in_stream);
		  switch (byte_read)
		    {
		    case ' ':	/* pen up */
		      dots_on = 0;
		      break;
		    case 'A':
		      x_incr++;
		      break;
		    case 'E':
		      x_incr++;
		      y_incr++;
		      break;
		    case 'D':
		      y_incr++;
		      break;
		    case 'F':
		      x_incr--;
		      y_incr++;
		      break;
		    case 'B':
		      x_incr--;
		      break;
		    case 'J':
		      x_incr--;
		      y_incr--;
		      break;
		    case 'H':
		      y_incr--;
		      break;
		    case 'I':
		      x_incr++;
		      y_incr--;
		      break;
		    case 'P':	/* pen down */
		      dots_on = 1;
		      break;
		    }
		}
	      ungetc (byte_read, in_stream);
	      /* printf(">.\n");*/
	      x += x_incr;
	      y += y_incr;
	      if (x < 0)
		x=0;
	      if (y < 0)
		y=0;
	      move (x, y);
	      text_x += x_incr;
	      text_y += y_incr;
	      if (text_x < 0)
		text_x=0;
	      if (text_y < 0)
		text_y=0;
	    }
	    break;
	  default:
	    fprintf (stderr, "Unrecognized `%c' <%o %o> ignored.\n",
		     byte_read, byte_read & 0x1f, byte_read);
	  }
      byte_read = read_byte (in_stream);
    }
  return;
}


char *progname;			/* argv[0] or the name of this program */

void
display_version ()
{
    (void) fprintf (stderr, "\
%s version %s, Copyright (C) 1989 Free Software Foundation, Inc.\n\
This software comes with ABSOLUTELY NO WARRANTY.\n\
See the file COPYING for details.\n",
		    progname, PACKAGE_VERSION);
}

/* Long options we recognize */
#define	ARG_NONE	0
#define	ARG_REQUIRED	1
#define	ARG_OPTIONAL	2

struct option	long_options[] = {
    { "fontsize",	ARG_REQUIRED,	NULL, 'f' },
    { "fontname",	ARG_REQUIRED,	NULL, 'F' },
    { "high-byte-first", ARG_NONE,	NULL, 'h' },
    { "low-byte-first",	ARG_NONE,	NULL, 'l' },
    { "help",		ARG_NONE,	NULL, 'H' },
    { "version",	ARG_NONE,	NULL, 'V' },
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
     char *argv[];
{
  int option;
  int opt_index;
  int errcnt = 0;		/* errors encountered */
  int show_version = 0;		/* remember to show version message */
  int show_usage = 0;		/* remember whether to output usage message. */
  int named_input = 0;		/* count named plot files on command line. */
  char *buffer;
  int  buffer_length;

  progname = argv[0];
  buffer_length = 1024;
  buffer = (char *) malloc (buffer_length);
  if (buffer <= (char *) 0)
    {
      perror ("malloc failed:");
      exit (-1);
    }

  openpl ();
  space (0, 0, 767, 767);

  while ((option = getopt_long(argc, argv, "-F:HVf:hl", long_options, &opt_index)) != EOF) {
      if (option == 0)
	option = long_options[opt_index].val;

      switch (option)
	{
	case 'f':
	  /* Sizes supported by X: 8, 10, 12, 14, 18, and 24. */
	  (void) fontsize (atoi (optarg));
	case 'F':
	  (void) fontname (optarg);
	  break;
	case 'h':		/* read high byte first */
	  output_high_byte_first = 1;
	  break;
	case 'l':		/* read low byte first */
	  output_high_byte_first = -1;
	  break;
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
	    read_plot (data_file, buffer, buffer_length);
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

  if (show_version)
    display_version ();

  if (errcnt > 0 || show_usage)
    display_help ();

  if (errcnt > 0 || show_usage)
    {
      closepl ();
      exit (errcnt > 0 ? 1 : 0);
    }


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
				  argv[0], argv[optind]);
		  continue;
		}
	    }
	  named_input++;
	  read_plot (stdin, buffer, buffer_length);

	  if (data_file != stdin) /* Don't close stdin */
	    fclose (data_file);
	}
    } /* endfor */

  if (!named_input)
    /* Read stdin if no files were named on the command line. */
    read_plot (stdin, buffer, buffer_length);

  closepl();
  return 0;
}
