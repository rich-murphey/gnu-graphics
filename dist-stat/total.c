 /*
 * $Header: /files1/home/toy/src/stat/RCS/total.c,v 1.5 90/09/01 17:15:13 toy Exp $
 * NAME
 *	total - sum total of vectors
 *
 * SYNOPSIS
 *	total [vectors...]
 *
 * DESCRIPTION
 *	Output is the sum total of the elements of the input
 *	vector(s).
 *
 * HISTORY
 * $Log:	total.c,v $
 * Revision 1.5  90/09/01  17:15:13  toy
 * Some more lint removal.
 *
 * Revision 1.4  90/09/01  17:02:27  toy
 * Use message function to print out messages.
 *
 * Revision 1.3  90/09/01  11:25:49  toy
 * Changed option characters.
 *
 * Revision 1.2  90/08/31  15:55:46  toy
 * Forgot to initialize total when we want a grand total.
 *
 * Revision 1.1  90/08/31  14:51:54  toy
 * Initial revision
 *
 */

#include <stdio.h>

#if	defined(__STDC__) || defined(__GNUC__)
#include <stddef.h>
#include <stdlib.h>
#endif

#include "gps.h"

#define	DEF_PROGNAME	"total"

static const char RCSID[] = "@(#) $Id: total.c,v 1.5 90/09/01 17:15:13 toy Exp $";

extern int getopt ();
extern int optind;
extern char *optarg;

const char *progname;

#define	OPT_STRING	"HTP:"

void
help ()
{
  (void) fprintf (stderr, "%s\n", RCSID);
  (void) fprintf (stderr, "Usage:  %s [-HT] [-P prec][vector ...]\n", progname);
  (void) fprintf (stderr, "\t-H\tThis help\n");
  (void) fprintf (stderr, "\t-T\tGrand total of all vectors listed\n");
  (void) fprintf (stderr, "\t\t(otherwise, print result for each vector)\n");
  (void) fprintf (stderr, "\t-P p\tSet number of significant digits to p\n");
}


#if	defined(__STDC__) || defined(__GNUC__)
double
do_total (FILE * fp)
#else
double
do_total (fp)
     FILE *fp;
#endif
{
  int rc;
  double x;
  double sum;

  sum = 0;

  do
    {
      rc = get_number (fp, &x);
      if (rc == 1)
	{
	  sum += x;
	}
      else if (rc == 0)
	{
	  message ("Error reading number\n");
	}
  } while (rc == 1);

  return sum;
}

int
main (argc, argv)
     int argc;
     char *argv[];
{
  int option;
  int total_sum;
  double sum;
  double total;

  progname = get_my_name (argv[0], DEF_PROGNAME);

  /*
   * Read options
   */

  total_sum = FALSE;
  while ((option = getopt (argc, argv, OPT_STRING)) != -1)
    {
      switch (option)
	{
	case 'H':		/* Help				 */
	  help ();
	  exit (0);
	  break;
	case 'T':		/* Grand total of all vectors	 */
	  total_sum = TRUE;
	  break;
	case 'P':		/* Precision			 */
	  (void) set_precision (atoi (optarg));
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
      print_number (stdout, do_total (stdin));
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
	      sum = do_total (stdin);
	      if (total_sum)
		{
		  total += sum;
		}
	      else
		{
		  print_number (stdout, sum);
		}
	    }
	  optind++;
	}			/* endwhile	 */

      if (total_sum)
	{
	  print_number (stdout, total);
	}
    }				/* endif	 */

  end_column (stdout);
  return 0;
}
