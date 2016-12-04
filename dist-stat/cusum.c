/*
 * $Header: /files1/home/toy/src/stat/RCS/cusum.c,v 1.3 90/09/04 16:04:46 toy Exp $
 * NAME
 *	cusum - cumulative (running) sum of a vector
 *
 * SYNOPSIS
 *	cusum [-H] [-P prec] [-c cols] [vectors...]
 *
 * DESCRIPTION
 *	Output is the cumulative (running) sum of the elements of
 *	the specified vector(s).
 *
 * HISTORY
 * $Log:	cusum.c,v $
 * Revision 1.3  90/09/04  16:04:46  toy
 * Use print_help_strings to print out the help strings.
 * Get the default precision from the environment variable STAT_PREC.
 *
 * Revision 1.2  90/09/01  17:01:38  toy
 * Use message function to print out messages.
 *
 * Revision 1.1  90/09/01  14:07:22  toy
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

#define	DEF_PROGNAME	"cusum"

static const char RCSID[] = "@(#) $Id: cusum.c,v 1.3 90/09/04 16:04:46 toy Exp $";

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
    "\t-c c\tSet number of output elements per line to n\n",
    "\tvec...\tData vectors, or stdin\n",
    "Print the cumulative (running) sum of the specified vectors.\n",
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
  double x;
  double cu_sum;

  cu_sum = 0;
  do
    {
      rc = get_number (fp, &x);
      if (rc == 1)
	{
	  cu_sum += x;
	  print_number (stdout, cu_sum);
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
  double total;

  progname = get_my_name (argv[0], DEF_PROGNAME);
  set_def_prec ();


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
