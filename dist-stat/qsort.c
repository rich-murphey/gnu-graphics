/*
 * $Header: /files1/home/toy/src/stat/RCS/qsort.c,v 1.3 90/09/04 17:40:42 toy Exp $
 * NAME
 *	qsort - sort the elements of the vector
 *
 * SYNOPSIS
 *	qsort [-Hr] [-P prec] [-c cols] [vectors...]
 *
 * DESCRIPTION
 *	Output is the sorted elements of the specified vector(s).
 *
 * HISTORY
 * $Log:	qsort.c,v $
 * Revision 1.3  90/09/04  17:40:42  toy
 * Set default precision from the environment variable STAT_PREC.
 * Use print_help_strings to print out help.
 *
 * Revision 1.2  90/09/02  17:14:26  toy
 * Use read_vector routine instead of do_read.
 *
 * Revision 1.1  90/09/01  18:09:03  toy
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

#define	DEF_PROGNAME	"qsort"

static const char RCSID[] = "@(#) $Id: qsort.c,v 1.3 90/09/04 17:40:42 toy Exp $";

extern int getopt ();
extern int optind;
extern char *optarg;

const char *progname;
int (*sort_function) ();

#define	OPT_STRING	"HP:c:r"

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
    "\t-r\tSort the elements in descending order\n",
    "Sort the vectors in ascending or descending order.\n",
    NULL
  };
  (void) fprintf (stderr, "%s\n", RCSID);
  (void) fprintf (stderr, "Usage:  %s [-H] [-P prec] [-c cols] [vector ...]\n", progname);
  print_help_strings (help_strings);
}

#if	defined(__STDC__) || defined(__GNUC__)
int
ascending (const void *a, const void *b)
#else
int
ascending (a, b)
     char *a;
     char *b;
#endif
{
  double *x;
  double *y;

  x = (double *) a;
  y = (double *) b;

  if (*x < *y)
    return -1;
  else if (*x > *y)
    return 1;
  else
    return 0;
}


#if	defined(__STDC__) || defined(__GNUC__)
int
descending (const void *a, const void *b)
#else
int
descending (a, b)
     char *a;
     char *b;
#endif
{
  double *x;
  double *y;

  x = (double *) a;
  y = (double *) b;

  if (*x < *y)
    return 1;
  else if (*x > *y)
    return -1;
  else
    return 0;
}

#if	defined(__STDC__) || defined(__GNUC__)
void
do_sort (double *vector, int rank)
#else
void
do_sort (vector, rank)
     double *vector;
     int rank;
#endif
{
  qsort ((char *) vector, rank, sizeof (double), sort_function);
  while (rank-- > 0)
    {
      print_number (stdout, *vector++);
    }
}



int
main (argc, argv)
     int argc;
     char *argv[];
{
  int option;
  int errcnt;
  int rank;
  double *vector;

  progname = get_my_name (argv[0], DEF_PROGNAME);
  set_def_prec ();

  /*
   * Read options
   */

  errcnt = 0;
  sort_function = ascending;

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
	case 'r':		/* Reverse order of sort	 */
	  sort_function = descending;
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
      do_sort (vector, rank);
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
	      do_sort (vector, rank);
	      free (vector);
	    }
	  optind++;
	}			/* endwhile	 */
    }				/* endif	 */

  end_column (stdout);
  return 0;
}
