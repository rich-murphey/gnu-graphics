/* plot2plot, a utility for converting Unix plot files into postscript.
   Copyright (C) 1989 Free Software Foundation, Inc.

Plot2plot is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY.  No author or distributor accepts responsibility to anyone for the
consequences of using it or for whether it serves any particular purpose or
works at all, unless he says so in writing.  Refer to the GNU General Public
License for full details.

Everyone is granted permission to copy, modify and redistribute plot2plot, but
only under the conditions described in the GNU General Public License.  A copy
of this license is supposed to have been given to you along with plot2plot so
you can know your rights and responsibilities.  It should be in a file named
COPYING.  Among other things, the copyright notice and this notice must be
preserved on all copies.  */

/* This file is the main routine for plot2plot.

   It includes code to read the plot file and call plot functions
   to draw the graphics. */

#include "sys-defines.h"
#include "libplot.h"

/* Long options we recognize */

#define	ARG_NONE	0
#define	ARG_REQUIRED	1
#define	ARG_OPTIONAL	2

struct option	long_options[] = {
    { "backward-compatible", ARG_NONE, NULL, 'b' },
    { "input-is-ascii", ARG_NONE, NULL, 'i' },
    { "output-as-ascii", ARG_NONE, NULL, 'o' },
    { "high-byte-first", ARG_NONE, NULL, 'h' },
    { "low-byte-first", ARG_NONE, NULL, 'l' },
    { "signed-input", ARG_NONE, NULL, 's' },
    { "unsigned-input", ARG_NONE, NULL, 'u' },
    { "help", ARG_NONE, NULL, 'H' },
    { "version", ARG_NONE, NULL, 'V' },
    { NULL, 0, NULL, 0}
};
    
enum plot_instruction
{
  ALABEL =	'T',
  ARC =		'a',
  CIRCLE =	'c',
  COLOR =	'C',
  COMMENT =	'#',
  CONT =	'n',
  ERASE =	'e',
  FILL =	'L',
  FONT =	'F',
  FONTSIZE =	'S',
  LABEL =	't',
  LINE =	'l',
  LINEMOD =	'f',
  MOVE =	'm',
  POINT =	'p',
  ROTATE =	'r',
  SPACE =	's',
};

/* a non-zero value specifies that the output be compatible with standard
   plot filters which do not support the GNU extensions such as
   adjusted labels. */
int backward_compatible = 0;

/* a non-zero value specifies that the input is in an ascii format.
   all strings and coordinates are preceded by a space and coordinates
   in ascii rather than binary. */
int ascii_input = 0;

/* a non-zero value specifies that the output is in an ascii format.
   all strings and coordinates are preceded by a space and coordinates
   in ascii rather than binary. */
int ascii_output = 0;

/* This flag specifies that the input contains unsigned (if zero) or
   signed (if nonzero) two byte intgers. The default is signed. */
int signed_input = 1;

/* read in two byte intgers with high_byte_first. The default is to read the low
   byte first. */
int high_byte_first = 0;

/* guess_byte_order is a flag which, if non-zero, indicated that the function
   find_byte_order should be used to guess the byte order for the input file. */
int guess_byte_order = 1;

/* swap_bytes returns the bottom two bytes of its integer argument with
   their bytes reversed. */

int
swap_bytes (x)
     int x;
{
  unsigned char a, b;
  a = x & 0xff;
  b = (x >> 8) & 0xff;
  return ((a << 8)|b);
}

/* This is a set of known values for the maximum x (abscissa) values
   specified in the plot size command for known devices.  Using this
   set we can construct a heuristic proceedure for recognizing plot
   files in which the bytes of two byte integers are reversed.  We can
   recognize these files by looking for size commands containing these
   known sizes in byte reversed form. The last entry should be 0. */
   
int known_size[32] = {
  504,				/* plot3d output 504x504 */
  2048,				/* versatek plotter 2048x2048 */
  2100,				/* plot3d output */
  3120,				/* Tektronix 4010 terminal 3120x3120 */
  4096,				/* GSI 300 terminal 4096x4096 */
  0				/* the last entry should be 0 */
  };

/* find_byte_order takes four integer arguments and matches third one
   against a set of known values (sizes, see above). If there is a match
   it merely returns.  If there is no match, it check each of the values
   again with the bottom two bytes reversed. If such a match is found, the
   bottom two bytes of each argument is reversed, the high_byte_first flag
   is inverted to indicated how two byte integers should be read and the
   runction returns. */
   
void
find_byte_order (x0, y0, x1, y1)
     int *x0, *y0, *x1, *y1;
{
  int i;
  for (i=0; known_size[i]!=0; i++)
    {
      if (*x1 == known_size[i])
	return;
    }
  /* now check to see if reversing the bytes allows a match... */
  for (i=0; known_size[i]!=0; i++)
    {
      if (*x1 == swap_bytes (known_size[i]))
	{
	  *x0 = swap_bytes( *x0);
	  *y0 = swap_bytes( *y0);
	  *x1 = swap_bytes( *x1);
	  *y1 = swap_bytes( *y1);
	  high_byte_first = ! high_byte_first;
	  return;
	}
    }

  /* finally, if none of this works, just assume that x1 > 128. */
  if ((*x1 < 128) && (swap_bytes (*x1) > 128))
    {
      *x0 = swap_bytes( *x0);
      *y0 = swap_bytes( *y0);
      *x1 = swap_bytes( *x1);
      *y1 = swap_bytes( *y1);
      high_byte_first = ! high_byte_first;
      return;
    }
}

/* Read a byte */

#define read_byte(stream) (getc (stream))

/* Read a coordinate - a two byte integer. */
  
int coord (input)
     FILE *input;
{
  int x;
  if (ascii_input)
    fscanf (input, " %d", &x);
  else
    {
      if ( high_byte_first )
	{
	  x = ((char) read_byte (input)) << 8; /* get sign from high byte */
	  x |= read_byte (input) & 0xFF; /* not from low byte */
	}
      else
	{
	  x = read_byte (input) & 0xFF; /* ingnore sign in low byte */
	  x |= ((char) read_byte (input)) << 8; /* get sign from high byte */
	}
      if ( ! signed_input )
	{
	  x &= 0xFFFF;
	}
    }
  return x;
}
  
/*  Read a string, change termination to null.
    note: string (buffer) reads a newline terminated string. */

void
read_string (input, buffer, buffer_length)
     FILE *input;
     char *buffer;
     int buffer_length;
{
  int length=0;
  char termination = '\n';
  char c = '\0';

  while (!feof (input))
    {
      if (length > buffer_length)
	{
	  buffer_length *= 2;
	  buffer = (char *) realloc (buffer, buffer_length);
	  if (buffer <= (char *) 0)
	    {
	      perror ("realloc failed:");
	      exit (-1);
	    }
	}
      c = read_byte (input);
      if (c == termination)
	break;
      buffer [length++] = c;
    }

  buffer [length] = '\0';	/*  null terminate label */
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
  char x_adjust, y_adjust;
  int x0, y0, x1, y1, x2, y2;
  int instruction;

  instruction = read_byte (in_stream);
  while (!feof (in_stream))
    {
      switch (instruction)
	{
	  /*  Note: we must get all but the last argument before calling to
	      ensure reading them in the proper order. */
	  
	case ALABEL:
	  x_adjust = read_byte (in_stream);
	  y_adjust = read_byte (in_stream); 
	  read_string (in_stream, buffer, buffer_length);
	  if (backward_compatible)
	    label (buffer);
	  else
	    alabel (x_adjust, y_adjust, buffer);
	  break;
	case ARC:
	  x0 = coord (in_stream);
	  y0 = coord (in_stream);
	  x1 = coord (in_stream);
	  y1 = coord (in_stream); 
	  x2 = coord (in_stream);
	  y2 = coord (in_stream); 
	  arc (x0, y0, x1, y1, x2, y2);
	  break;
	case CIRCLE:
	  x0 = coord (in_stream);
	  y0 = coord (in_stream);
	  x1 = coord (in_stream);
	  circle (x0, y0, x1);
	  break;
	case COLOR:
	  x0 = coord (in_stream);
	  y0 = coord (in_stream);
	  x1 = coord (in_stream);
	  if (!backward_compatible)
	    color (x0, y0, x1);
	  break;
	case COMMENT:
	  read_string (in_stream, buffer, buffer_length);
	  break;
	case CONT:
	  x0 = coord (in_stream);
	  y0 = coord (in_stream);
	  cont (x0, y0);
	  break;
	case ERASE:
	  erase ();
	  break;
	case FILL:
	  x0 = coord (in_stream);
	  if (!backward_compatible)
	    fill (x0);
	  break;
	case FONT:
	  read_string (in_stream, buffer, buffer_length);
	  if (!backward_compatible)
	    fontname (buffer);
	  break;
	case FONTSIZE:
	  x0 = coord (in_stream);
	  if (!backward_compatible)
	    fontsize (x0);
	  break;
	case LABEL:
	  read_string (in_stream, buffer, buffer_length);
	  label (buffer);
	  break;
	case LINE:
	  x0 = coord (in_stream);
	  y0 = coord (in_stream);
	  x1 = coord (in_stream);
	  y1 = coord (in_stream); 
	  line (x0, y0, x1, y1);
	  break;
	case LINEMOD:
	  read_string (in_stream, buffer, buffer_length);
	  linemod (buffer);
	  break;
	case MOVE:
	  x0 = coord (in_stream);
	  y0 = coord (in_stream);
	  move (x0, y0);
	  break;
	case POINT:
	  x0 = coord (in_stream);
	  y0 = coord (in_stream);
	  point (x0, y0);
	  break;
	case ROTATE:
	  x0 = coord (in_stream);
	  y0 = coord (in_stream);
	  x1 = coord (in_stream);
	  if (!backward_compatible)
	    rotate (x0, y0, x1);
	  break;
	case SPACE:
	  x0 = coord (in_stream);
	  y0 = coord (in_stream);
	  x1 = coord (in_stream);
	  y1 = coord (in_stream); 
	  if (guess_byte_order)
	    find_byte_order(&x0, &y0, &x1, &y1);
	  space (x0, y0, x1, y1);
	  break;
        case '\n':
	  if (ascii_input)
	    break;
	default:
	  fprintf (stderr, "Unrecognized plot command `%c' ignored.\n", instruction);
	}
      instruction = read_byte (in_stream);
    }
  return;
}


void
read_plot_ascii_out (in_stream, buffer, buffer_length)
     FILE *in_stream;
     char *buffer;
     int  buffer_length;
{
  char x_adjust, y_adjust;
  int x0, y0, x1, y1, x2, y2;
  int instruction;

  instruction = read_byte (in_stream);
  while (!feof (in_stream))
    {
      switch (instruction)
	{
	  /*  Note: we must get all but the last argument before calling to
	      ensure reading them in the proper order. */
	  
	case ALABEL:
	  x_adjust = read_byte (in_stream);
	  y_adjust = read_byte (in_stream); 
	  read_string (in_stream, buffer, buffer_length);
	  if (backward_compatible)
	    printf("%c%s\n", LABEL, buffer);
	  else
	    printf("%c%c%c%s\n", ALABEL, x_adjust, y_adjust, buffer);
	  break;
	case ARC:
	  x0 = coord (in_stream);
	  y0 = coord (in_stream);
	  x1 = coord (in_stream);
	  y1 = coord (in_stream); 
	  x2 = coord (in_stream);
	  y2 = coord (in_stream); 
	  printf("%c %d %d %d %d %d %d\n", ARC, x0, y0, x1, y1, x2, y2);
	  break;
	case CIRCLE:
	  x0 = coord (in_stream);
	  y0 = coord (in_stream);
	  x1 = coord (in_stream);
	  printf("%c %d %d %d\n", CIRCLE, x0, y0, x1);
	  break;
	case COLOR:
	  x0 = coord (in_stream);
	  y0 = coord (in_stream);
	  x1 = coord (in_stream);
	  if (!backward_compatible)
	  printf("%c %d %d %d\n", COLOR, x0, y0, x1);
	  break;
	case CONT:
	  x0 = coord (in_stream);
	  y0 = coord (in_stream);
	  printf("%c %d %d\n", CONT, x0, y0);
	  break;
	case ERASE:
	  printf("%c\n", ERASE);
	  break;
	case FILL:
	  x0 = coord (in_stream);
	  if (!backward_compatible)
	    printf("%c %d\n", FILL, x0);
	  break;
	case FONT:
	  read_string (in_stream, buffer, buffer_length);
	  if (!backward_compatible)
	    printf("%c%s\n", FONT, buffer);
	  break;
	case FONTSIZE:
	  x0 = coord (in_stream);
	  if (!backward_compatible)
	    printf("%c %d\n", FONTSIZE, x0);
	  break;
	case LABEL:
	  read_string (in_stream, buffer, buffer_length);
	  printf("%c%s\n", LABEL, buffer);
	  break;
	case LINE:
	  x0 = coord (in_stream);
	  y0 = coord (in_stream);
	  x1 = coord (in_stream);
	  y1 = coord (in_stream); 
	  printf("%c %d %d %d %d\n", LINE, x0, y0, x1, y1);
	  break;
	case LINEMOD:
	  read_string (in_stream, buffer, buffer_length);
	  printf("%c%s\n", LINEMOD, buffer);
	  break;
	case MOVE:
	  x0 = coord (in_stream);
	  y0 = coord (in_stream);
	  printf("%c %d %d\n", MOVE, x0, y0);
	  break;
	case POINT:
	  x0 = coord (in_stream);
	  y0 = coord (in_stream);
	  printf("%c %d %d\n", POINT, x0, y0);
	  break;
	case ROTATE:
	  x0 = coord (in_stream);
	  y0 = coord (in_stream);
	  x1 = coord (in_stream);
	  if (!backward_compatible)
	    printf("%c %d %d %d\n", ROTATE, x0, y0, x1);
	  break;
	case SPACE:
	  x0 = coord (in_stream);
	  y0 = coord (in_stream);
	  x1 = coord (in_stream);
	  y1 = coord (in_stream); 
	  if (guess_byte_order)
	    find_byte_order(&x0, &y0, &x1, &y1);
	  printf("%c %d %d %d %d\n", SPACE, x0, y0, x1, y1);
	  break;
        case '\n':
	  if (ascii_input)
	    break;
	default:
	  fprintf (stderr, "Unrecognized plot command `%c' ignored.\n", instruction);
	}
      instruction = read_byte (in_stream);
    }
  return;
}

void
display_version()
{
    (void) fprintf (stderr, "\
plot2plot version %s, Copyright (C) 1989 Free Software Foundation, Inc.\n\
This software comes with ABSOLUTELY NO WARRANTY.\n\
See the file COPYING for details.\n",
		    PACKAGE_VERSION);
}

void
  display_help()
{
    (void) fprintf(stderr, "\
Usage:  plot2plot [-biohlsuCHVW]\n\
        [+backward-compatible] [+input-is-ascii] [+output-as-ascii]\n\
        [+high-byte-first] [+low-byte-first] [+signed-input]\n\
        [+unsigned-input] [+help] [+version] [data_files]\n");
}


int
main (argc, argv)
     int argc;
     char *argv[];
{
  char *buffer;
  int  buffer_length;
  int  opened=0;		/* remember whether we have opened output. */
  int  show_help = 0;		/* remember to show a help message */
  int  show_version = 0;	/* remember to show the version */
  int  errcnt = 0;
  int  option;			/* Option character */
  int  opt_index;		/* Long option index */

  buffer_length = 1024;
  buffer = (char *) malloc (buffer_length);
  if (buffer <= (char *) 0)
    {
      perror ("malloc failed:");
      exit (-1);
    }

  while ((option = getopt_long(argc, argv, "-HVbiohlsu", long_options, &opt_index)) != EOF) {
      if (option == 0)
	option = long_options[opt_index].val;

      switch (option) {
      case 'b':		/* Backward compatible */
	  backward_compatible = 1;
	  break;
      case 'i':		/* Ascii input	*/
	  ascii_input = 1;
	  break;
      case 'o':		/* Ascii output */
	  ascii_output = 1;
	  break;
      case 'h':		/* High byte first */
	  guess_byte_order = 0;
	  high_byte_first = 1;
	  break;
      case 'l':		/* Low byte first */
	  guess_byte_order = 0;
	  high_byte_first = 0;
	  break;
      case 's':		/* Signed */
	  signed_input = 1;
	  break;
      case 'u':		/* Unsigned */
	  signed_input = 0;
	  break;
      case 'H':		/* Help */
	  show_help = 1;
	  break;
      case 'V':		/* Version */
	  show_version = 1;
	  break;
      default:
	  errcnt++;
	  break;
      }
  }

  if (show_version)
    display_version();

  if (errcnt > 0 || show_help)
    display_help();

  if (errcnt > 0 || show_help)
    exit(errcnt > 0 ? 1 : 0);
  
  if (optind >= argc) {
      /* Read from stdin */

      if (ascii_output)
	read_plot_ascii_out(stdin, buffer, buffer_length);
      else
	read_plot(stdin, buffer, buffer_length);
      opened++;
      openpl();
  } else {
      FILE	*filep;

      for ( ; optind < argc; optind++) {
	  if (strcmp(argv[optind], "-") == 0) {
	      filep = stdin;
	  } else {
	      filep = fopen(argv[optind], "r");
	      if (filep == NULL) {
		  (void) fprintf(stderr, "%s:  inaccessible or nonexistent file ignored:  `%s'\n",
				 argv[0], argv[optind]);
	      } else {
		  if (opened++)
		    openpl();
		  if (ascii_output)
		    read_plot_ascii_out(filep, buffer, buffer_length);
		  else
		    read_plot(filep, buffer, buffer_length);
		  /* Close the file, but not if it's stdin */
		  if (filep != stdin)
		    fclose(filep);
	      }
	  }
      }	/* endfor */
  }
      

  if (opened)
    closepl();

  return 0;
}
