/*
 * $Header: /files1/home/toy/src/stat/RCS/gas.c,v 1.3 90/11/04 18:18:05 toy Exp $
 * NAME
 *	gas - generate additive sequence
 *
 * SYNOPSIS
 *	gas [-H] [-P prec] [-c cols] [-i interval] [-n number] [-s start] [-t terminate]
 *
 * DESCRIPTION
 *	Output is an additive sequence
 *
 * HISTORY
 * $Log:	gas.c,v $
 * Revision 1.3  90/11/04  18:18:05  toy
 * Fixed help message.
 *
 * Revision 1.2  90/09/04  16:04:48  toy
 * Use print_help_strings to print out the help strings.
 * Get the default precision from the environment variable STAT_PREC.
 *
 * Revision 1.1  90/09/02  16:33:31  toy
 * Initial revision
 *
 */

#include <stdio.h>
#include <math.h>

#if	defined(__STDC__) || defined(__GNUC__)
#include <stddef.h>
#include <stdlib.h>
#endif

#include "gps.h"

#define	DEF_PROGNAME	"gas"

static const char RCSID[] = "@(#) $Id: gas.c,v 1.3 90/11/04 18:18:05 toy Exp $";

extern int getopt ();
extern int optind;
extern char *optarg;

extern long random PARMS ((void));
extern void srandom PARMS ((unsigned));

const char *progname;

#define	OPT_STRING	"HP:c:i:n:s:t:"

void
help ()
{

  static const char *help_strings[] =
  {
    "\t\t[-m mult] [-n num] [-s seed]\n",
    "\t-H\tThis help\n",
    "\t-P p\tSet number of significant digits to p.  If not specified\n",
    "\t\tuse value of STAT_PREC from environment, if available.  Else\n",
    "\t\tuse default.\n",
    "\t-c c\tSet number of output elements per line to n\n",
    "\t-i i\tInterval between numbers\n",
    "\t-n n\tNumber of values to generate (default is 10, if -t is not\n",
    "\t\tgiven)\n",
    "\t-s s\tStarting value (default is 1)\n",
    "\t-t t\tTerminating value (default is infinity, but -n usually\n",
    "\t\tterminates the sequence)\n",
    "Generate an additive sequence of specified length and spacing\n",
    "Examples:\n",
    "\tgas                     generates the numbers 1 to 10\n",
    "\tgas -s.01 -t.05 -i .01  generates .01, .02, .03, .04, .05\n",
    "\tgas -s3 -i2 -n4 -t5     generates 3, 5, 3, 5\n",
    NULL
  };

  (void) fprintf (stderr, "%s\n", RCSID);
  (void) fprintf (stderr, "\
Usage:  %s [-H] [-P prec] [-c cols] [-i interval] [-n num] [-s start]\n\
\t[-t end]\n",
		  progname);
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
  int got_terminate;
  int got_number;
  int number;			/* Number of elements	 */
  double interval;		/* Step size		 */
  double start;			/* Starting point	 */
  double terminate;		/* Ending point		 */
  double x;

  progname = get_my_name (argv[0], DEF_PROGNAME);
  set_def_prec ();
  set_def_prec ();

  errcnt = 0;
  interval = 1;
  number = 10;
  start = 1;
  got_number = got_terminate = FALSE;

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
	case 'c':		/* Number of columns		 */
	  (void) set_max_columns (atoi (optarg));
	  break;
	case 'i':		/* Interval			 */
	  if (sscanf (optarg, "%lf", &interval) != 1)
	    {
	      message ("cannot read -i value: `%s'\n", optarg);
	      errcnt++;
	    }
	  break;
	case 'n':		/* Number of values to generate	 */
	  if (sscanf (optarg, "%d", &number) != 1)
	    {
	      message ("cannot read -n value: `%s'\n", optarg);
	      errcnt++;
	    }
	  else
	    {
	      got_number = TRUE;
	    }
	  break;
	case 's':		/* Start			 */
	  if (sscanf (optarg, "%lf", &start) != 1)
	    {
	      message ("cannot read -s value: `%s'\n", optarg);
	      errcnt++;
	    }
	  break;
	case 't':		/* Terminating value		 */
	  if (sscanf (optarg, "%lf", &terminate) != 1)
	    {
	      message ("cannot read -t value: `%s'\n", optarg);
	      errcnt++;
	    }
	  else
	    {
	      got_terminate = TRUE;
	    }
	  break;
	default:
	  break;
	}			/* endswitch	 */
    }				/* endwhile	 */

  /*
   * Check options
   */

  if (number <= 0)
    {
      message ("number of elements must be positive\n");
      errcnt++;
    }

  /*
   * Exit if any errors found
   */

  if (errcnt > 0)
    {
      exit (1);
    }

  /*
   * Figure out how many numbers to generate
   */

  if (got_terminate && !got_number)
    {
      number = floor (fabs ((terminate - start) / interval) + 1 + 0.5);
      printf ("number = %d\n", number);
    }

  /*
   * Generate the specified sequence
   */

  if (got_terminate)
    {
      for (k = 0; k < number; k++)
	{
	  x = start + fmod (k * interval, terminate - start + interval);
	  print_number (stdout, x);
	}
    }
  else
    {
      for (k = 0; k < number; k++)
	{
	  x = start + k * interval;
	  print_number (stdout, x);
	}
    }

  end_column (stdout);
  return 0;
}
