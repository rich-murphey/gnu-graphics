/*
 * $Header: /files1/home/toy/src/stat/RCS/siline.c,v 1.3 90/09/04 18:34:13 toy Exp $
 * NAME
 *	siline - generate a line given the slope and intercept
 *
 * SYNOPSIS
 *	siline [-H] [-P prec] [-c cols] [-i intercept] [-n nsamps] [-s slope]
 *		[vectors ...]
 *
 * DESCRIPTION
 *	Output is a line with the given slope and intercept
 *
 * HISTORY
 * $Log:	siline.c,v $
 * Revision 1.3  90/09/04  18:34:13  toy
 * Wasn't printing out the help strings!
 *
 * Revision 1.2  90/09/04  17:40:51  toy
 * Set default precision from the environment variable STAT_PREC.
 * Use print_help_strings to print out help.
 *
 * Revision 1.1  90/09/02  17:52:57  toy
 * Initial revision
 *
 */

#include <stdio.h>

#if	defined(__STDC__) || defined(__GNUC__)
#include <stddef.h>
#include <stdlib.h>
#endif

#include "gps.h"

#define	DEF_PROGNAME	"siline"

static const char RCSID[] = "@(#) $Id: siline.c,v 1.3 90/09/04 18:34:13 toy Exp $";

extern int getopt ();
extern int optind;
extern char *optarg;

extern long random PARMS ((void));
extern void srandom PARMS ((unsigned));

const char *progname;

#define	OPT_STRING	"HP:c:i:n:s:"

void
help ()
{
  static const char *help_strings[] =
  {
    "\t-H\tThis help\n",
    "\t-P p\tSet number of significant digits to p.  If not specified\n",
    "\t\tuse value of STAT_PREC from environment, if available.  Else\n",
    "\t\tuse default.\n",
    "\t-c c\tSet number of output elements per line to n\n",
    "\t-i i\tIntercept of line, default = 0\n",
    "\t-n n\tNumber of positive integers for x\n",
    "\t-s s\tSlope of line, default = 1\n",
    "The output consists of values from a line with the given\n",
    "slope and intercept:  y = slope * x + intercept.  The x values\n",
    "are taken from the vectors, if given, or from stdin.  If -n is\n",
    "given, the x values are the sequence of positive integers from 1\n",
    "to n.\n",
    NULL
  };

  (void) fprintf (stderr, "%s\n", RCSID);
  (void) fprintf (stderr, "Usage:  %s [-H] [-P prec] [-c cols] [-i intercept] [-n nsamp] [-s slope]\n", progname);
  (void) fprintf (stderr, "\t\t[vectors ...]\n");
  print_help_strings (help_strings);
}


int
main (argc, argv)
     int argc;
     char *argv[];
{
  int option;
  int errcnt;
  int k;
  int got_nsamp;
  int nsamp;
  double intercept;
  double slope;
  double x;

  progname = get_my_name (argv[0], DEF_PROGNAME);
  set_def_prec ();

  /*
   * Read options
   */

  errcnt = 0;
  got_nsamp = FALSE;
  intercept = 0;
  slope = 1;

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
	case 'c':		/* Number of columns		 */
	  (void) set_max_columns (atoi (optarg));
	  break;
	case 'i':		/* Intercept			 */
	  if (sscanf (optarg, "%lf", &intercept) != 1)
	    {
	      message ("cannot read -i value: `%s'\n", optarg);
	      errcnt++;
	    }
	  break;
	case 'n':		/* Number of positive integers	 */
	  if (sscanf (optarg, "%d", &nsamp) != 1)
	    {
	      message ("cannot read -n value: `%s'\n", optarg);
	      errcnt++;
	    }
	  else
	    {
	      got_nsamp = TRUE;
	    }
	  break;
	case 's':		/* Slope of line		 */
	  if (sscanf (optarg, "%lf", &slope) != 1)
	    {
	      message ("cannot read -s value: `%s'\n", optarg);
	      errcnt++;
	    }
	  break;
	default:
	  break;
	}			/* endswitch	 */
    }				/* endwhile	 */

  /*
   * Exit if any errors found
   */
  if (errcnt > 0)
    {
      exit (1);
    }

  /*
   * Generate the desired line
   */

  if (got_nsamp)
    {
      for (k = 1; k <= nsamp; k++)
	{
	  print_number (stdout, intercept + slope * k);
	}
    }
  else
    {
      while (optind < argc)
	{
	  if (freopen (argv[optind], "r", stdin) == NULL)
	    {
	      message ("cannot open file `%s'\n", argv[optind]);
	      exit (1);
	    }
	  while (get_number (stdin, &x) == 1)
	    {
	      print_number (stdout, intercept + slope * x);
	    }
	  optind++;
	}
    }

  end_column (stdout);
  return 0;
}
