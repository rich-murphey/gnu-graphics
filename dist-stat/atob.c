/*
 * $Header: /files1/home/toy/src/stat/RCS/atob.c,v 1.1 90/11/04 17:08:28 toy Exp $
 *
 * NAME
 *	atob - ASCII to binary converter
 *
 * SYNOPSIS
 *	atob [-o{}] [files]
 *
 * DESCRIPTION
 *
 * HISTORY
 * $Log:	atob.c,v $
 * Revision 1.1  90/11/04  17:08:28  toy
 * Initial revision
 *
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#if	defined(__STDC__) || defined(STDC_HEADERS)
#include <stddef.h>
#include <stdlib.h>
#endif

#include "gps.h"

extern int getopt ();
extern int optind;
extern char *optarg;

static const char RCSID[] = "@(#)$Id: atob.c,v 1.1 90/11/04 17:08:28 toy Exp $";

const char *progname;

#define	DEF_PROGNAME	"atob"
#define	OPT_STRING	"H?o:"
#define	OUTPUT_FORMATS	"bsifdBSI"
#define	DEF_OUTPUT_FORM	's'

#if	defined(__STDC__)
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
    "\t-o o\tBinary output format:  bsifd.  The character means\n",
    "\t\tbyte, short int, int, float, or double.  Use upper case\n",
    "\t\tletter for unsigned version.\n",
    "Read the given file or stdin for a sequence of numbers.  The\n",
    "output will be the binary version of the given numbers.\n",
    NULL
  };

  (void) fprintf (stderr, "%s\n\n", RCSID);
  (void) fprintf (stderr, "Usage:  %s [-H] [-o{%s}] [files]\n",
		  progname, OUTPUT_FORMATS);
  print_help_strings (help_strings);
}

#if	defined(__STDC__)
void
dump_data (int output_form)
#else
void
dump_data (output_form)
     int output_form;
#endif
{
  char byte;
  short short_val;
  int int_val;
  unsigned char ubyte;
  unsigned short ushort_val;
  unsigned int uint_val;
  float float_val;
  double value;

  /*
   * Read each number in, convert it to the appropriate type
   * and save it.
   */

  while (get_number (stdin, &value) == 1)
    {
      switch (output_form)
	{
	case 'b':		/* Byte size		*/
	  byte = (char) value;
	  fwrite (&byte, 1, 1, stdout);
	  break;
	case 'B':		/* Unsigned byte	*/
	  ubyte = (unsigned char) value;
	  fwrite (&ubyte, 1, 1, stdout);
	  break;
	case 's':		/* Short int		*/
	  short_val = (short) value;
	  fwrite (&short_val, sizeof (short), 1, stdout);
	  break;
	case 'S':		/* Unsigned short	*/
	  ushort_val = value;
	  fwrite (&ushort_val, sizeof (ushort_val), 1, stdout);
	  break;
	case 'i':		/* Int			*/
	  int_val = (int) value;
	  fwrite (&int_val, sizeof (int), 1, stdout);
	  break;
	case 'I':		/* Unsigned int		*/
	  uint_val = (unsigned int) value;
	  fwrite (&uint_val, sizeof (uint_val), 1, stdout);
	  break;
	case 'f':		/* Float		*/
	  float_val = (float) value;
	  fwrite (&float_val, sizeof (float), 1, stdout);
	  break;
	case 'd':		/* Double		*/
	  fwrite (&value, 1, sizeof (double), stdout);
	  break;
	default:
	  message ("Fatal error:  unknown output form:  `%c' (%x)\n",
		   output_form, output_form);
	  exit (2);
	}
    }
}



int
main (argc, argv)
     int argc;
     char *argv[];
{
  char output_format;
  int option;
  int errcnt;

  progname = get_my_name (argv[0], DEF_PROGNAME);


  /*
   * Process options
   */

  errcnt = 0;
  output_format = DEF_OUTPUT_FORM;

  while ((option = getopt (argc, argv, OPT_STRING)) != -1)
    {
      switch (option)
	{
	case 'H':		/* Help			*/
	case '?':
	  help ();
	  exit (0);
	case 'o':		/* Binary output form	*/
	  output_format = *optarg;
	  break;
	default:
	  break;
	}
    }

  /*
   * Check options
   */

  if (strchr (OUTPUT_FORMATS, output_format) == NULL)
    {
      message ("Unknown input format character: `%c' (0x%x)\n", output_format, (int) output_format);
      errcnt++;
    }

  if (errcnt > 0)
    {
      help ();
      exit (1);
    }

  /*
   * Read each file and write it in the desired format.
   * Use stdin if no files given.
   */

  if (optind >= argc)
    {
      dump_data (output_format);
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
	      dump_data (output_format);
	    }
	  optind++;
	}
    }

  return 0;
}
