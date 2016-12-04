/*
 * $Header: /files1/home/toy/src/stat/RCS/hilo.c,v 1.4 90/09/04 17:39:53 toy Exp $
 * NAME
 *	hilo - hi and/or low values
 *
 * SYNOPSIS
 *	hilo [-H] [-P prec] [-hl] [-o char] [vectors...]
 *
 * DESCRIPTION
 *	Output is the cumulative (running) sum of the elements of
 *	the specified vector(s).
 *
 * HISTORY
 * $Log:	hilo.c,v $
 * Revision 1.4  90/09/04  17:39:53  toy
 * Set default precision from the environment variable STAT_PREC.
 * Use print_help_strings to print out help.
 *
 * Revision 1.3  90/09/02  17:12:10  toy
 * Fixed revision history
 *
 * Revision 1.2  90/09/01  17:01:53  toy
 * Use message function to print out messages.
 *
 * Revision 1.1  90/09/01  14:33:46  toy
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

#define	DEF_PROGNAME	"hilo"

static const char RCSID[] = "@(#) $Id: hilo.c,v 1.4 90/09/04 17:39:53 toy Exp $";

extern int getopt ();
extern int optind;
extern char *optarg;

const char *progname;

#define	OPT_STRING	"HP:hlo:"

void
help ()
{
  static const char *help_strings[] =
  {
    "\t-H\tThis help\n",
    "\t-P p\tSet number of significant digits to p.  If not specified\n",
    "\t\tuse value of STAT_PREC from environment, if available.  Else\n",
    "\t\tuse default.\n",
    "\t-h\tOnly output high value\n",
    "\t-l\tOnly onput low value\n",
    "\t-o c\tOutput values in option form with `c' prepended\n",
    "Print the max and min values of a vector.  Use -h or -l to\n",
    "specify only the max or min value, respectively.  If -o c is\n",
    "given, the output is of the form '-ch high -cl low'.\n",
    "\n",
    "The high and/or low values are printed for each vector\n",
    NULL
  };
  (void) fprintf (stderr, "%s\n", RCSID);
  (void) fprintf (stderr, "Usage:  %s [-H] [-P prec] [-c cols] [vector ...]\n", progname);
  print_help_strings (help_strings);
}

#if	defined(__STDC__) || defined(__GNUC__)
void
do_func (FILE * fp, double *low, double *high)
#else
void
do_func (fp, low, high)
     FILE *fp;
     double *low;
     double *high;
#endif
{
  int rc;
  double x;

  do
    {
      rc = get_number (fp, &x);
      if (rc == 1)
	{
	  if (*low > x)
	    {
	      *low = x;
	    }
	  if (*high < x)
	    {
	      *high = x;
	    }
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
  char opt_char;
  int option;
  int output_lo;
  int output_hi;
  int precision;		/* Output precision	*/
  double low;			/* Lowest number	*/
  double high;			/* Highest number	*/


  progname = get_my_name (argv[0], DEF_PROGNAME);
  set_def_prec ();

  /*
   * We need to get the default precision for later
   */
  precision = set_precision (-1);
  output_lo = output_hi = TRUE;
  opt_char = '\0';

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
	  precision = atoi (optarg);
	  if (precision < 0)
	    {
	      precision = set_precision (-1);	/* Get default	*/
	    }
	  (void) set_precision (precision);
	  break;
	case 'h':		/* Output high value only	 */
	  output_lo = FALSE;
	  break;
	case 'l':		/* Output low value only	 */
	  output_hi = FALSE;
	  break;
	case 'o':		/* Output in option form	 */
	  opt_char = *optarg;
	  break;
	default:
	  break;
	}			/* endswitch	 */
    }				/* endwhile	 */

  /*
   * If no files are listed, use stdin.  Otherwise use
   * the given files.
   */

  low = HUGE;
  high = -HUGE;

  if (optind == argc)
    {
      do_func (stdin, &low, &high);
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
	      do_func (stdin, &low, &high);
	    }
	  optind++;
	}			/* endwhile	 */
    }				/* endif	 */

  if (opt_char != '\0')
    {
      if (output_hi)
	{
	  printf ("-%ch%.*g ", opt_char, precision, high);
	}
      if (output_lo)
	{
	  printf ("-%cl%.*g ", opt_char, precision, low);
	}
    }
  else
    {
      if (output_hi)
	{
	  print_number (stdout, high);
	  end_column (stdout);
	}
      if (output_lo)
	{
	  print_number (stdout, low);
	}
    }


  end_column (stdout);
  return 0;
}
