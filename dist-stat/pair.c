/*
 * $Header: /files1/home/toy/src/stat/RCS/pair.c,v 1.2 90/09/10 12:45:22 toy Exp $
 * NAME
 *	pair - pair element groups
 *
 * SYNOPSIS
 *	pair [-H] [-P prec] [-c cols] [-F base] [-x reps] [vectors...]
 *
 * DESCRIPTION
 * HISTORY
 * $Log:	pair.c,v $
 * Revision 1.2  90/09/10  12:45:22  toy
 * Removed some debugging statements.
 * Forgot to initialize variable j before using it!
 *
 * Revision 1.1  90/09/06  18:20:52  toy
 * Initial revision
 *
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#if	defined(__STDC__) || defined(__GNUC__)
#include <stddef.h>
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#include "gps.h"

#define	DEF_PROGNAME	"pair"

static const char RCSID[] = "@(#) $Id: pair.c,v 1.2 90/09/10 12:45:22 toy Exp $";

extern int getopt ();
extern int optind;
extern char *optarg;

const char *progname;

#define	OPT_STRING	"HP:c:F:x:"

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
    "\t-P p\tSet number of significant digits to p.  If not specified\n",
    "\t\tuse value of STAT_PREC from environment, if available.  Else\n",
    "\t\tuse default.\n",
    "\t-c c\tSet number of output elements per line to n\n",
    "\t-F F\tBase (x) vector is read from file F.  If not specified, the\n",
    "\t\tdata is read from stdin.\n",
    "\t-x x\tNumber of elements taken from the base for each one element\n",
    "\t\ttaken from the vector.\n",
    "Output is a vector with elements taken alternately from a base\n",
    "and from a vector.  The base vector is specified with the F\n",
    "option, or is implicitly defined to come from stdin.\n",
    NULL
  };

  (void) fprintf (stderr, "%s\n", RCSID);
  print_help_strings (help_strings);
}


int
main (argc, argv)
     int argc;
     char *argv[];
{
  char *base;
  int k;
  int option;
  int errcnt;
  int done;
  int nfiles;
  int base_reps;
  double x;
  FILE *base_fp;
  FILE **file_list;

  set_def_prec ();
  progname = get_my_name (argv[0], DEF_PROGNAME);

  errcnt = 0;
  base = NULL;
  base_fp = NULL;
  base_reps = 1;

  /*
   * Read options
   */

  while ((option = getopt (argc, argv, OPT_STRING)) != -1)
    {
      switch (option)
	{
	case 'H':		/* Help				 */
	  help ();
	  exit (0);
	  break;
	case 'P':		/* Precision			 */
	  (void) set_precision (atoi (optarg));
	  break;
	case 'c':		/* Number of output columns	 */
	  (void) set_max_columns (atoi (optarg));
	  break;
	case 'F':		/* Base vector file		 */
	  base = optarg;
	  break;
	case 'x':		/* number of reps from base	 */
	  base_reps = atoi (optarg);
	  break;
	default:
	  break;
	}			/* endswitch	 */
    }				/* endwhile	 */

  /*
   * If a base vector is given, read it from there.
   * Otherwise read it from stdin.
   */

  if (base == NULL)
    {
      base_fp = stdin;
    }
  else
    {
      base_fp = fopen (base, "r");
      if (base_fp == NULL)
	{
	  message ("cannot open base file `%s'\n", base);
	  errcnt++;
	}
    }

  /*
   * Exit on errors
   */

  if (errcnt > 0)
    {
      exit (1);
    }

  /*
   * Open any other files listed on the command line
   */

  nfiles = argc - optind;

  if (nfiles <= 0)
    {
      nfiles = 1;
      file_list = (FILE **) malloc (nfiles * sizeof (FILE *));
      assert (file_list != NULL);
      *file_list = stdin;
    }
  else
    {
      int j;
      file_list = (FILE **) malloc (nfiles * sizeof (FILE *));
      assert (file_list != NULL);
      j = 0;
      for (k = 0; k < nfiles; k++, optind++)
	{
	  file_list[j] = fopen (argv[optind], "r");
	  if (file_list[j] == NULL)
	    {
	      message ("cannot open for reading:  `%s'\n", argv[optind]);
	    }
	  else
	    {
	      j++;
	    }
	}			/* endfor	 */
      nfiles = j;
    }

  /*
   * Read some elements from the base and print them out.
   * Then print an element from the given files.
   */

  done = FALSE;
  do
    {
      for (k = 0; k < base_reps; k++)
	{
	  if (get_number (base_fp, &x) == 1)
	    {
	      print_number (stdout, x);
	    }
	  else
	    {
	      done = TRUE;
	    }
	}

      if (!done)
	{
	  for (k = 0; k < nfiles; k++)
	    {
	      if (get_number (file_list[k], &x) == 1)
		{
		  print_number (stdout, x);
		}
	      else
		{
		  done = TRUE;
		}
	    }			/* endfor	 */
	}
  } while (!done);

  end_column (stdout);
  return 0;
}
