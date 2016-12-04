/*
 * $Header: /files1/home/toy/src/stat/RCS/btoa.c,v 1.3 90/11/04 17:12:05 toy Exp $
 * NAME
 *	btoa - binary to ASCII
 *
 * SYNOPSIS
 *	btoa [-H] [-P prec] [-i{cisudf}] [-o{cdx}] [-c cols] [files ...]
 *
 * DESCRIPTION
 *	Read in the data using the specified format and print
 *	out the data in human-readable form according to the
 *	desired output format.
 *
 *	The -c option specifies the number of columns to be used
 *	in printing the output.  The default is 8.  The -P option
 *	specifies the default precision for printing floating point
 *	numbers.
 *
 *	The recognized input formats are specified using the -i
 *	option.  The formats are:
 *
 *		c	- signed char
 *		C	- unsigned char
 *		i	- int
 *		I	- unsigned int
 *		s	- short int
 *		S	- unsigned short int
 *		l	- long
 *		L	- unsigned long
 *		f	- float
 *		d	- double
 *
 *	The default input format is s, short int.
 *
 *	The numbers are printed out according the to -o option.
 *	The valid formats are:
 *
 *		a	- ASCII, or octal if not printable
 *		b	- byte values, hex
 *		c	- characters
 *		d	- decimal integers
 *		o	- octal integers
 *		u	- unsigned integers
 *		x	- hexadecimal format
 *		f	- floating point
 *
 *	Suitable defaults are chosen for each input format.  The
 *	default is d, decimal integers.
 *
 * HISTORY
 * $Log:	btoa.c,v $
 * Revision 1.3  90/11/04  17:12:05  toy
 * errcnt was not initialized
 *
 * Revision 1.2  90/09/11  18:24:02  toy
 * Added support for an "a" (ASCII) output format.
 * Fixed a few bugs in printing numbers in columns.
 *
 * Revision 1.1  90/09/10  20:16:53  toy
 * Initial revision
 *
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#if	defined(__STDC__) || defined(__GNUC__)
#include <stddef.h>
#include <stdlib.h>
#endif

#include "gps.h"

extern int getopt ();
extern int optind;
extern char *optarg;

static const char RCSID[] = "@(#) $Id: btoa.c,v 1.3 90/11/04 17:12:05 toy Exp $";

const char *progname;

#define	DEF_PROGNAME	"btoa"
#define	OPT_STRING	"Hc:i:o:"
#define	DEF_INPUT_FORM	's'
#define	DEF_OUTPUT_FORM	'd'
#define	INPUT_FORMATS	"csilfdCSIL"
#define	OUTPUT_FORMATS	"abcduoxf"

#if	defined(__STDC__) || defined(__GNUC__)
void
help (void)
#else
void
help ()
#endif
{
  static const char *help_strings[] =
  {
    "\t-H\tThis help\n",
    "\t-c c\tNumber of output columns per line\n",
    "\t-i i\tThe input is assumed to have the format specified.\n",
    "\t\tThe option character means char, short, int, long, float,\n",
    "\t\tor double.  Use upper case character for unsigned versions.\n",
    "\t-o o\tOutput format.  The option character means ascii,\n",
    "\t\tbyte (hex), char, decimal, unsigned, octal, hex, floating\n",
    "Read the given file or stdin assuming the input is in the\n",
    "specified format.  This in then printed out in the specified\n",
    "output format.  This is useful for printing out a speech file.\n",
    NULL
  };

  (void) fprintf (stderr, "%s\n\n", RCSID);
  (void) fprintf (stderr, "Usage:  %s [-H] [-c cols] [-i{%s}] [-o{%s}] [files]\n",
		  progname, INPUT_FORMATS, OUTPUT_FORMATS);
  print_help_strings (help_strings);
  (void) fprintf (stderr, "The default input format is `%c'; the default output, `%c'.\n",
		  DEF_INPUT_FORM, DEF_OUTPUT_FORM);
}


#if	defined(__STDC__) || defined(__GNUC__)
void
print_it (int input_form, int output_form)
#else
void
print_it (input_form, output_form)
     int input_form;
     int output_form;
#endif
{
  char hformat[BUFSIZ];
  char oformat[BUFSIZ];
  int k;
  int input_size;
  long value;
  unsigned long uvalue;
  union
  {
    char Char;
    unsigned char Uchar;
    short Short;
    unsigned short Ushort;
    int Int;
    unsigned Uint;
    long Long;
    unsigned long Ulong;
    float Float;
    double Double;
    char buffer[sizeof (double)];
  } number;

  /*
   * Initialize number buffer
   */
  memset (number.buffer, 0, sizeof (double));

  /*
   * Figure out the size of the input item
   */

  switch (input_form)
    {
    case 'c':			/* Character type	 */
    case 'C':
      input_size = sizeof (char);
      break;
    case 'i':			/* Integer type		 */
    case 'I':
      input_size = sizeof (int);
      break;
    case 's':			/* Short int type	 */
    case 'S':
      input_size = sizeof (short);
      break;
    case 'l':			/* Long int type	 */
    case 'L':
      input_size = sizeof (long);
      break;
    case 'f':			/* Float type		 */
      input_size = sizeof (float);
      break;
    case 'd':			/* Double type		 */
      input_size = sizeof (double);
      break;
    default:
      message ("Fatal internal error:  print_it size char = %x\n", (int) input_form);
      exit (2);
    }				/* endswitch	 */

  if ((input_form == 'f') || (input_form == 'd'))
    {
      while (fread (&number.buffer, input_size, 1, stdin) == 1)
	{
	  if (output_form == 'b')
	    {
	      for (k = 0; k < input_size; k++)
		{
		  printf ("%02hx", ((unsigned short) number.buffer[k]) & 0xff);
		}
	      next_column (stdout);
	    }
	  else
	    {
	      print_number (stdout, (input_form == 'f') ? number.Float : number.Double);
	    }
	}			/* endwhile 	 */
    }
  else
    {

      /*
       * Compute the correct field width for printing hex
       * and octal numbers
       */

      (void) sprintf (hformat, "%%0%dlx", 2 * input_size);
      (void) sprintf (oformat, "%%0%dlo", (8 * input_size + 2) / 3);

      /*
       * Read in the desired element
       */

      while (fread (&number.buffer, input_size, 1, stdin) == 1)
	{

	  /*
           * Convert the input number into a long or
           * unsigned long
           */

	  if (input_size == sizeof (unsigned long))
	    {
	      uvalue = number.Ulong;
	      value = number.Long;
	    }
	  else if (input_size == sizeof (unsigned int))
	    {
	      uvalue = (unsigned long) number.Uint;
	      value = (long) number.Int;
	    }
	  else if (input_size == sizeof (unsigned short))
	    {
	      uvalue = (unsigned long) number.Ushort;
	      value = (long) number.Short;
	    }
	  else
	    {
	      uvalue = (unsigned long) number.Uchar;
	      value = (long) number.Char;
	    }

	  /*
           * Print out the number in the desired format
           */

	  switch (output_form)
	    {
	    case 'a':		/* ASCII		 */
	      for (k = 0; k < input_size; k++)
		{
		  if (isascii (number.buffer[k]) && isprint (number.buffer[k]))
		    {
		      putchar (number.buffer[k]);
		    }
		  else
		    {
		      printf ("\\%03o", ((unsigned short) number.buffer[k]) & 0xff);
		    }
		  next_column (stdout);
		}
	      break;
	    case 'b':		/* Byte			 */
	      for (k = 0; k < input_size; k++)
		{
		  printf ("%02hx", ((unsigned short) number.buffer[k]) & 0xff);
		}
	      next_column (stdout);
	      break;
	    case 'c':		/* Characters		 */
	      for (k = 0; k < input_size; k++)
		{
		  putchar ((int) number.buffer[k]);
		}
	      next_column (stdout);
	      break;
	    case 'd':		/* Decimal integers	 */
	      printf ("%ld", value);
	      next_column (stdout);
	      break;
	    case 'u':		/* Unsigned integer	 */
	      printf ("%lu", uvalue);
	      next_column (stdout);
	      break;
	    case 'o':		/* Octal format		 */
	      printf (oformat, uvalue);
	      next_column (stdout);
	      break;
	    case 'x':		/* Hex format		 */
	      printf (hformat, uvalue);
	      next_column (stdout);
	      break;
	    case 'f':		/* Floating format	 */
	      print_number (stdout, (double) value);
	      next_column (stdout);
	      break;
	    default:
	      message ("Fatal error:  unknown output format:  `%c' (%0x)\n", output_form, (int) output_form);
	      exit (2);
	    }			/* endswitch	 */
	}			/* endwhile	 */
    }				/* endif	 */
}



int
main (argc, argv)
     int argc;
     char *argv[];
{
  char input_format;
  char output_format;
  int option;
  int errcnt;

  progname = get_my_name (argv[0], DEF_PROGNAME);
  set_def_prec ();
  set_max_columns (8);

  input_format = DEF_INPUT_FORM;
  output_format = DEF_OUTPUT_FORM;
  errcnt = 0;

  while ((option = getopt (argc, argv, OPT_STRING)) != -1)
    {
      switch (option)
	{
	case 'H':		/* Help			 */
	  help ();
	  exit (0);
	  break;
	case 'c':		/* Columns		 */
	  set_max_columns (atoi (optarg));
	  break;
	case 'i':		/* Input format		 */
	  input_format = *optarg;
	  break;
	case 'o':		/* Output format	 */
	  output_format = *optarg;
	  break;
	default:
	  break;
	}			/* endswitch	 */
    }				/* endwhile	 */

  /*
   * Check options
   */

  if (strchr (INPUT_FORMATS, input_format) == NULL)
    {
      message ("Unknown input format character: `%c' (0x%x)\n", input_format, (int) input_format);
      errcnt++;
    }
  if (strchr (OUTPUT_FORMATS, output_format) == NULL)
    {
      message ("Unknown output format character:  `%c' (0x%x)\n", output_format, (int) output_format);
      errcnt++;
    }
  if (errcnt > 0)
    {
      help ();
      exit (1);
    }

  /*
   * Print each specified file, or use stdin, if non
   * given
   */

  if (optind >= argc)
    {
      print_it (input_format, output_format);
    }
  else
    {
      while (optind < argc)
	{
	  if (freopen (argv[optind], "r", stdin) == NULL)
	    {
	      message ("cannot open for reading:  `%s'\n", argv[optind]);
	    }
	  else
	    {
	      print_it (input_format, output_format);
	    }
	  optind++;
	}
    }

  end_column (stdout);
  return 0;
}
