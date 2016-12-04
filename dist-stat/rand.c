/*
 * $Header: /files1/home/toy/src/stat/RCS/rand.c,v 1.6 90/09/10 12:24:04 toy Exp $
 * NAME
 *	rand - generate a random sequence
 *
 * SYNOPSIS
 *	rand [-H] [-P prec] [-c cols] [-h high] [-l low] [-m mult] [-n num] [-s seed]
 *
 * DESCRIPTION
 *	Output is a set of random values.
 *
 * HISTORY
 * $Log:	rand.c,v $
 * Revision 1.6  90/09/10  12:24:04  toy
 * Some help strings didn't have a comma in them.
 *
 * Revision 1.5  90/09/04  17:40:44  toy
 * Set default precision from the environment variable STAT_PREC.
 * Use print_help_strings to print out help.
 *
 * Revision 1.4  90/09/02  17:12:28  toy
 * Fixed revision history
 *
 * Revision 1.3  90/09/01  17:02:24  toy
 * Use message function to print out messages.
 *
 * Revision 1.2  90/09/01  12:56:38  toy
 * Wasn't handling -n option correctly.
 *
 * Revision 1.1  90/09/01  12:54:24  toy
 * Initial revision
 */

#include <stdio.h>

#if	defined(__STDC__) || defined(__GNUC__)
#include <stddef.h>
#include <stdlib.h>
#endif

#include "gps.h"

#define	DEF_PROGNAME	"rand"

static const char RCSID[] = "@(#) $Id: rand.c,v 1.6 90/09/10 12:24:04 toy Exp $";

extern int getopt ();
extern int optind;
extern char *optarg;

extern long random PARMS ((void));
extern void srandom PARMS ((unsigned));

const char *progname;

#define	OPT_STRING	"HP:c:h:l:m:n:s:"

void
help ()
{
  static const char *help_strings[] =
  {
    "\t-H\tThis help.\n",
    "\t-P p\tSet number of significant digits to p.  If not specified\n",
    "\t\tuse value of STAT_PREC from environment, if available.  Else\n",
    "\t\tuse default.\n",
    "\t-c c\tSet number of output elements per line to n.\n",
    "\t-h h\tHighest number is h.  Default is 1.\n",
    "\t-l l\tLowest number is l.  Default is 0.\n",
    "\t-m m\tMultiplier is m.  Default is determined from h and l.\n",
    "\t\tCannot be specified with either -h or -l.\n",
    "\t-n n\tNumber of values to generate.\n",
    "\t-s s\tSet seed to s.  Default is 1.\n",
    "Generate a set of uniformly distributed random numbers with\n",
    "the given range.  The default range is [0, 1).  If a multiplier\n",
    "is given, the numbers are multiplied appropriately.\n",
    NULL
  };
  (void) fprintf (stderr, "%s\n", RCSID);
  (void) fprintf (stderr, "Usage:  %s [-H] [-P prec] [-c cols] [-h high] [-l low]\n", progname);
  (void) fprintf (stderr, "\t\t[-m mult] [-n num] [-s seed]\n");
  print_help_strings (help_strings);
}


int
main (argc, argv)
     int argc;
     char *argv[];
{
  int option;
  int errcnt;
  int got_high;
  int got_low;
  int got_mult;
  int num;
  unsigned seed;
  double low;
  double high;
  double mult;
  double x;

  progname = get_my_name (argv[0], DEF_PROGNAME);
  set_def_prec ();

  /*
   * Read options
   */

  errcnt = 0;
  got_high = got_low = got_mult = FALSE;
  low = 0;
  high = 1;
  num = 10;
  seed = 1;

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
	case 'h':		/* High			 */
	  if (sscanf (optarg, "%lf", &high) != 1)
	    {
	      message ("cannot read -h value: `%s'\n", optarg);
	      errcnt++;
	    }
	  else
	    {
	      got_high = TRUE;
	    }
	  break;
	case 'l':		/* Low			 */
	  if (sscanf (optarg, "%lf", &low) != 1)
	    {
	      message ("cannot read -l value: `%s'\n", optarg);
	      errcnt++;
	    }
	  else
	    {
	      got_low = TRUE;
	    }
	  break;
	case 'm':		/* Multiplier		 */
	  if (sscanf (optarg, "%lf", &mult) != 1)
	    {
	      message ("cannot read -m value: `%s'\n", optarg);
	      errcnt++;
	    }
	  else
	    {
	      got_mult = TRUE;
	    }
	  break;
	case 'n':		/* Number of samples	 */
	  if (sscanf (optarg, "%d", &num) != 1)
	    {
	      message ("cannot read -n value: `%s'\n", optarg);
	      errcnt++;
	    }
	  break;
	case 's':		/* Seed			 */
	  if (sscanf (optarg, "%u", &seed) != 1)
	    {
	      message ("cannot read -s value: `%s'\n", optarg);
	      errcnt++;
	    }
	  else
	    {
	      srandom (seed);
	    }
	  break;
	default:
	  break;
	}			/* endswitch	 */
    }				/* endwhile	 */

  /*
   * Check options
   */

  if (got_mult && (got_low || got_high))
    {
      message ("cannot specify limits with a multiplier\n");
      errcnt++;
    }				/* endif	 */

  /*
   * Exit if any errors found
   */
  if (errcnt > 0)
    {
      exit (1);
    }
  if (!got_mult)
    {
      mult = high - low;
    }

  /*
   * Generate the desired random numbers
   */

  while (num-- > 0)
    {
      x = random () / 2147483648.0;	/* 2**31 = 2147483648	 */
      x = low + mult * x;
      print_number (stdout, x);
    }

  end_column (stdout);
  return 0;
}
