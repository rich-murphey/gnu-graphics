/*
 * $Header: /files1/home/toy/src/stat/RCS/var.c,v 1.3 90/09/04 17:40:53 toy Exp $
 * NAME
 *	var - variance of the elements of a vector
 *
 * SYNOPSIS
 *	var [-HB] [-P prec] [-c cols] [vectors...]
 *
 * DESCRIPTION
 *	Output is the variance of the elements of the specified vector(s).
 *
 * BUGS
 *	The entire vector is read into memory to compute the variance.
 *	This is done for accuracy (but is it really necessary?).
 *
 * HISTORY
 * $Log:	var.c,v $
 * Revision 1.3  90/09/04  17:40:53  toy
 * Set default precision from the environment variable STAT_PREC.
 * Use print_help_strings to print out help.
 *
 * Revision 1.2  90/09/02  17:14:28  toy
 * Use read_vector routine instead of do_read.
 *
 * Revision 1.1  90/09/02  15:44:55  toy
 * Initial revision
 *
 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#if	defined(__STDC__) || defined(__GNUC__)
#include <stddef.h>
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#include "gps.h"

#define	DEF_PROGNAME	"var"

static const char RCSID[] = "@(#) $Id: var.c,v 1.3 90/09/04 17:40:53 toy Exp $";

extern int getopt ();
extern int optind;
extern char *optarg;

const char *progname;

#define	OPT_STRING	"HBP:c:"

void
help ()
{
  static const char *help_strings[] =
  {
    "\t-H\tThis help\n",
    "\t-B\tUse unbiased variance estimate\n",
    "\t-P p\tSet number of significant digits to p\n",
    "\t-c c\tSet number of output elements per line to n\n",
    "The variance of each vector is printed.\n",
    NULL
  };

  (void) fprintf (stderr, "%s\n", RCSID);
  (void) fprintf (stderr, "Usage:  %s [-H] [-P prec] [-c cols] [vector ...]\n", progname);
  print_help_strings (help_strings);
}

#if	defined(__STDC__) || defined(__GNUC__)
void
do_var (double *vector, int rank, int biased)
#else
void
do_var (vector, rank, biased)
     double *vector;
     int rank;
     int biased;
#endif
{
  int k;
  double mean;
  double var;
  double *ptr;

  /*
   * First compute the mean
   */

  mean = 0;
  ptr = vector;
  for (k = 0; k < rank; k++)
    {
      mean += *ptr++;
    }				/* endfor	 */
  mean /= (double) rank;

  /*
   * Now compute variance
   */

  var = 0;
  ptr = vector;
  for (k = 0; k < rank; k++)
    {
      double diff;
      diff = *ptr++ - mean;
      var += diff * diff;
    }				/* endfor	 */

  var /= (biased) ? rank : rank - 1;
  print_number (stdout, var);
}



int
main (argc, argv)
     int argc;
     char *argv[];
{
  int option;
  int errcnt;
  int rank;
  int biased;
  double *vector;

  progname = get_my_name (argv[0], DEF_PROGNAME);
  set_def_prec ();

  errcnt = 0;
  biased = TRUE;
  (void) set_max_columns (1);

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
	case 'B':		/* Use unbiased estimate	 */
	  biased = FALSE;
	  break;
	default:
	  break;
	}			/* endswitch	 */
    }				/* endwhile	 */

  if (errcnt > 0)
    {
      exit (1);
    }

  /*
   * If no files are listed, use stdin.  Otherwise use
   * the given files
   */

  if (optind == argc)
    {
      vector = read_vector (stdin, &rank);
      do_var (vector, rank, biased);
      free (vector);
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
	      vector = read_vector (stdin, &rank);
	      do_var (vector, rank, biased);
	      free (vector);
	    }
	  optind++;
	}			/* endwhile	 */
    }				/* endif	 */

  end_column (stdout);
  return 0;
}
