/*
 * $Header: /files1/home/toy/src/stat/RCS/rank.c,v 1.2 90/09/04 17:40:46 toy Exp $
 * NAME
 *	rank - rank (number of elements) of a vector
 *
 * SYNOPSIS
 *	rank [-H] [-P prec] [-c cols] [vectors...]
 *
 * DESCRIPTION
 *	Output is the rank of the the specified vector(s).
 *
 * HISTORY
 * $Log:	rank.c,v $
 * Revision 1.2  90/09/04  17:40:46  toy
 * Set default precision from the environment variable STAT_PREC.
 * Use print_help_strings to print out help.
 *
 * Revision 1.1  90/09/01  17:52:10  toy
 * Initial revision
 *
 * Revision 1.2  90/09/01  17:01:38  toy
 * Use message function to print out messages.
 *
 * Revision 1.1  90/09/01  14:07:22  toy
 * Initial revision
 *
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

#define	DEF_PROGNAME	"rank"

static const char RCSID[] = "@(#) $Id: rank.c,v 1.2 90/09/04 17:40:46 toy Exp $";

extern int getopt ();
extern int optind;
extern char *optarg;

const char *progname;

#define	OPT_STRING	"HP:c:"

void
help ()
{
  static const char *help_strings[] =
  {
    "\t-H\tThis help\n",
    "\t-P p\tSet number of significant digits to p.  If not specified\n",
    "\t\tuse value of STAT_PREC from environment, if available.  Else\n",
    "\t\tuse default.\n",
    "\t-c c\tSet number of output elements per line to n.  (Default = 1.)\n",
    "The rank (number of elements) of the vector is printed.\n",
    "If more than one vector is given, the rank of each vector\n",
    "is printed.\n",
    NULL
  };
  (void) fprintf (stderr, "%s\n", RCSID);
  (void) fprintf (stderr, "Usage:  %s [-H] [-P prec] [-c cols] [vector ...]\n", progname);
  print_help_strings (help_strings);
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
  long rank;
  double x;

  rank = 0;
  do
    {
      rc = get_number (fp, &x);
      if (rc == 1)
	{
	  rank++;
	}
      else if (rc == 0)
	{
	  message ("Error reading number\n");
	}
  } while (rc == 1);

  print_number (stdout, (double) rank);
}


int
main (argc, argv)
     int argc;
     char *argv[];
{
  int option;
  double total;

  progname = get_my_name (argv[0], DEF_PROGNAME);
  set_def_prec ();

  /*
   * Read options
   */

  (void) set_max_columns (1);

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
	default:
	  break;
	}			/* endswitch	 */
    }				/* endwhile	 */

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
      total = 0;
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
