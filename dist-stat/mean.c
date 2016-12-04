/*
 * $Header: /files1/home/toy/src/stat/RCS/mean.c,v 1.4 90/09/10 12:22:11 toy Exp $
 * NAME
 *	mean - mean of a vector
 *
 * SYNOPSIS
 *	mean [-H] [-P prec] [-c cols] [vectors...]
 *
 * DESCRIPTION
 *	Output is the possibly trimmed mean of the elements of
 *	the specified vector(s).
 *
 * HISTORY
 * $Log:	mean.c,v $
 * Revision 1.4  90/09/10  12:22:11  toy
 * Forgot to print out help strings.
 * Fixed error in function declaration when not using
 * ANSI or GNU C.
 *
 * Revision 1.3  90/09/04  17:40:34  toy
 * Set default precision from the environment variable STAT_PREC.
 * Use print_help_strings to print out help.
 *
 * Revision 1.2  90/09/02  17:14:04  toy
 * Use read_vector routine instead of do_read.
 *
 * Revision 1.1  90/09/01  16:53:35  toy
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

#define	DEF_PROGNAME	"mean"

static const char RCSID[] = "@(#) $Id: mean.c,v 1.4 90/09/10 12:22:11 toy Exp $";

extern int getopt ();
extern int optind;
extern char *optarg;

const char *progname;

#define	OPT_STRING	"HP:c:f:p:n:"

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
    "\t-f f\tTrim (1/f)*r elements from each end, where r\n",
    "\t\tis the rank of the vector\n",
    "\t-p p\tTrim p*r elements, where 0 <= p < 0.5\n",
    "\t-n n\tTrim n elements\n",
    "Compute the mean of the given vector.  Elements may be optionally\n",
    "trimmed from each end before computing the mean.\n",
    "\n",
    "A separate mean is printed for each specified vector\n",
    NULL
  };

  (void) fprintf (stderr, "%s\n", RCSID);
  (void) fprintf (stderr, "Usage:  %s [-H] [-P prec] [-c cols] [vector ...]\n", progname);
  print_help_strings (help_strings);
}

#if	defined(__STDC__) || defined(__GNUC__)
void
do_mean (double *vector, int rank, int ntrim)
#else
void
do_mean (vector, rank, ntrim)
     double *vector;
     int rank;
     int ntrim;
#endif
{
  int k;
  double sum;

  /*
   * Trim off the appropriate number of elements from
   * both ends of the vector
   */

  rank -= 2 * ntrim;
  vector += ntrim;

  if (rank > 0)
    {
      sum = 0;
      for (k = 0; k < rank; k++)
	{
	  sum += *vector++;
	}
      print_number (stdout, sum / rank);
    }				/* endif	 */
}



int
main (argc, argv)
     int argc;
     char *argv[];
{
  int option;
  int errcnt;
  int got_f_opt;
  int got_p_opt;
  int got_n_opt;
  int rank;
  int trim_number;
  double trim_frac;
  double trim_percent;
  double *vector;

  progname = get_my_name (argv[0], DEF_PROGNAME);
  set_def_prec ();

  /*
   * Read options
   */

  errcnt = 0;
  got_f_opt = got_p_opt = got_n_opt = FALSE;
  trim_number = 0;

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
	case 'c':		/* Number of output columsn	 */
	  (void) set_max_columns (atoi (optarg));
	  break;
	case 'f':		/* Fraction to trim		 */
	  got_f_opt = TRUE;
	  trim_frac = atof (optarg);
	  break;
	case 'p':		/* Percentage to trim		 */
	  got_p_opt = TRUE;
	  trim_percent = atof (optarg);
	  break;
	case 'n':		/* Number to trim		 */
	  got_n_opt = TRUE;
	  trim_number = atoi (optarg);
	  break;
	default:
	  break;
	}			/* endswitch	 */
    }				/* endwhile	 */

  if ((got_f_opt && got_p_opt) || (got_f_opt && got_n_opt) || (got_p_opt && got_n_opt))
    {
      message ("-f, -p, and -n options are mutually exclusive\n");
      errcnt++;
    }
  if (got_f_opt && (trim_frac <= 1))
    {
      message ("trim fraction must be greater than 1\n");
      errcnt++;
    }
  if (got_p_opt && ((trim_percent <= 0) || (trim_percent >= 0.5)))
    {
      message ("trim percentage must be between 0 and .5\n");
      errcnt++;
    }
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
      if (got_f_opt)
	{
	  trim_number = rank / trim_frac + 0.5;
	}
      else if (got_p_opt)
	{
	  trim_number = rank * trim_percent + 0.5;
	}
      do_mean (vector, rank, trim_number);
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
	      if (got_f_opt)
		{
		  trim_number = rank / trim_frac + 0.5;
		}
	      else if (got_p_opt)
		{
		  trim_number = rank * trim_percent + 0.5;
		}
	      do_mean (vector, rank, trim_number);
	      free (vector);
	    }
	  optind++;
	}			/* endwhile	 */
    }				/* endif	 */

  end_column (stdout);
  return 0;
}
