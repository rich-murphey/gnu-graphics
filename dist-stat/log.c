/*
 * $Header: /files1/home/toy/src/stat/RCS/log.c,v 1.3 90/09/04 17:40:29 toy Exp $
 * NAME
 *	log - logarithm
 *
 * SYNOPSIS
 *	log [-H] [-P prec] [-c cols] [-b base] [vectors...]
 *
 * DESCRIPTION
 *	Output is the logarithm of the elements of
 *	the specified vector(s).
 *
 * HISTORY
 * $Log:	log.c,v $
 * Revision 1.3  90/09/04  17:40:29  toy
 * Set default precision from the environment variable STAT_PREC.
 * Use print_help_strings to print out help.
 *
 * Revision 1.2  90/09/01  17:01:56  toy
 * Use message function to print out messages.
 *
 * Revision 1.1  90/09/01  14:56:43  toy
 * Initial revision
 *
 */

#include <stdio.h>
#include <math.h>
#include <string.h>

#if	defined(__STDC__) || defined(__GNUC__)
#include <stddef.h>
#include <stdlib.h>
#endif

#include "gps.h"

#define	DEF_PROGNAME	"log"

static const char RCSID[] = "@(#) $Id: log.c,v 1.3 90/09/04 17:40:29 toy Exp $";

extern int getopt ();
extern int optind;
extern char *optarg;

const char *progname;
double log_base;		/* Natural log of the base of the log	 */
double (*function) PARMS ((double));

#define	OPT_STRING	"HP:c:b:"

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
    "\t-b b\tSet base of the logarithm to b\n",
    "Compute the base b log of the elements of the given vectors\n",
    NULL
  };
  (void) fprintf (stderr, "%s\n", RCSID);
  (void) fprintf (stderr, "Usage:  %s [-H] [-P prec] [-c cols] [-b base] [vector ...]\n", progname);
  print_help_strings (help_strings);
}


#if	defined(__STDC__) || defined(__GNUC__)
double
Log (double x)
#else
double
Log (x)
     double x;
#endif
{
  return log (x) / log_base;
}

#if	defined(__STDC__) || defined(__GNUC__)
void
do_func (FILE * fp)
#else
void
do_func (fp)
     FILE *fp;
#endif
{
  int rc;
  double x;

  do
    {
      rc = get_number (fp, &x);
      if (rc == 1)
	{
	  print_number (stdout, (*function) (x));
	}
      else if (rc == 0)
	{
	  message ("Error reading number\n");
	}
  } while (rc == 1);
}


int
main (argc, argv)
     int argc;
     char *argv[];
{
  int option;
  int use_natural;
  double base;

  progname = get_my_name (argv[0], DEF_PROGNAME);
  set_def_prec ();

  /*
   * Read options
   */

  use_natural = TRUE;

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
	case 'b':		/* Base of the logs		 */
	  base = atof (optarg);
	  use_natural = FALSE;
	  if ((base <= 0) || (base = 1))
	    {
	      message ("invalid base specified: `%s'\n", optarg);
	      exit (1);
	    }
	  break;
	default:
	  break;
	}			/* endswitch	 */
    }				/* endwhile	 */

  if (use_natural)
    {
      function = log;
    }
  else
    {
      log_base = log (base);
      function = Log;
    }

  /*
   * If no files are listed, use stdin.  Otherwise use
   * the given files
   */

  if (optind == argc)
    {
      do_func (stdin);
    }
  else
    {
      while (optind < argc)
	{
	  if (freopen (argv[optind], "r", stdin) == NULL)
	    {
	      message ("Cannot open `%s'\n", argv[optind]);
	    }
	  else
	    {
	      do_func (stdin);
	    }
	  optind++;
	}			/* endwhile	 */
    }				/* endif	 */

  end_column (stdout);
  return 0;
}
