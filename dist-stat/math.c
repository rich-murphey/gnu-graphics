/*
 * $Header: /files1/home/toy/src/stat/RCS/math.c,v 1.5 90/09/11 17:00:14 toy Exp $
 * NAME
 *	abs, ceil, floor, exp, cos, sin, tan, asin, acos, atan, gamma
 *		- math function applied to the elements of a vector
 *
 * SYNOPSIS
 *	abs [-H] [-P prec] [-c cols] [vectors...]
 *	ceil [-H] [-P prec] [-c cols] [vectors...]
 *	floor [-H] [-P prec] [-c cols] [vectors...]
 *	exp [-H] [-P prec] [-c cols] [vectors...]
 *	cos [-H] [-P prec] [-c cols] [vectors...]
 *	sin [-H] [-P prec] [-c cols] [vectors...]
 *	tan [-H] [-P prec] [-c cols] [vectors...]
 *	asin [-H] [-P prec] [-c cols] [vectors...]
 *	acos [-H] [-P prec] [-c cols] [vectors...]
 *	atan [-H] [-P prec] [-c cols] [vectors...]
 *	gamma [-H] [-P prec] [-c cols] [vectors...]
 *
 * DESCRIPTION
 *	Output is the appropriate math function of the elements of
 *	the specified vector(s).
 *
 * HISTORY
 * $Log:	math.c,v $
 * Revision 1.5  90/09/11  17:00:14  toy
 * Forget to set the default precision.
 * Error messages should print the vector name.
 *
 * Revision 1.4  90/09/04  17:40:32  toy
 * Set default precision from the environment variable STAT_PREC.
 * Use print_help_strings to print out help.
 *
 * Revision 1.3  90/09/01  17:42:16  toy
 * Use default name if we have an unknown program name.
 *
 * Revision 1.2  90/09/01  17:01:58  toy
 * Use message function to print out messages.
 *
 * Revision 1.1  90/09/01  14:00:45  toy
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

#define	DEF_PROGNAME	"[abs|ceil|floor|exp|trig|gamma]"

/*
 * Figure out what the name of the gamma (really log gamma)
 * function is.  Sun likes lgamma, but System V uses gamma.
 */

#if	defined(sun) || defined(USE_LGAMMA)
#define	GAMMA	lgamma
#elif	defined(SYSV) || defined(USG)
#define	GAMMA	gamma
#else /* The default is gamma	 */
#define	GAMMA	gamma
#endif

static const char RCSID[] = "@(#) $Id: math.c,v 1.5 90/09/11 17:00:14 toy Exp $";

extern int getopt ();
extern int optind;
extern char *optarg;

const char *progname;
const char *vector_name;

double (*function) PARMS ((double));

#define	OPT_STRING	"HP:c:"

#if	defined(__STDC__) || defined(__GNUC__)
void
help (void)
#else
void
help ()
#endif
{
  static const char *help_strings[] =
  {
    "\t-H\tThis help\n",
    "\t-P p\tSet number of significant digits to p.  If not specified\n",
    "\t\tuse value of STAT_PREC from environment, if available.  Else\n",
    "\t\tuse default.\n",
    "\t-c c\tSet number of output elements per line to n\n",
    NULL
  };
  (void) fprintf (stderr, "%s\n", RCSID);
  (void) fprintf (stderr, "Usage:  %s [-H] [-P prec] [-c cols] [vector ...]\n", progname);
  print_help_strings (help_strings);
  (void) fprintf (stderr, "Apply the function `%s' to the specified vectors, or stdin.\n", progname);
}


#if	defined(__STDC__) || defined(__GNUC__)
double
identity (double x)
#else
double
identity (x)
     double x;
#endif
{
  return x;
}


#if	defined(__STDC__) || defined(__GNUC__)
double
Gamma (double x)
#else
double
Gamma (x)
     double x;
#endif
{
  double lg;
  extern int signgam;

  lg = GAMMA (x);

  return signgam * exp (lg);
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
  int cnt;
  double x;

  cnt = 0;
  do
    {
      cnt++;
      rc = get_number (fp, &x);
      if (rc == 1)
	{
	  print_number (stdout, (*function) (x));
	}
      else if (rc == 0)
	{
	  message ("Error reading number (element %d) from file `%s'.\n",
		   cnt, vector_name);
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
   * From the program name figure out what function we
   * want to run
   */

  if (strcmp (progname, "abs") == 0)
    {
      function = fabs;
    }
  else if (strcmp (progname, "ceil") == 0)
    {
      function = ceil;
    }
  else if (strcmp (progname, "floor") == 0)
    {
      function = floor;
    }
  else if (strcmp (progname, "exp") == 0)
    {
      function = exp;
    }
  else if (strcmp (progname, "cos") == 0)
    {
      function = cos;
    }
  else if (strcmp (progname, "sin") == 0)
    {
      function = sin;
    }
  else if (strcmp (progname, "tan") == 0)
    {
      function = tan;
    }
  else if (strcmp (progname, "asin") == 0)
    {
      function = asin;
    }
  else if (strcmp (progname, "acos") == 0)
    {
      function = acos;
    }
  else if (strcmp (progname, "atan") == 0)
    {
      function = atan;
    }
  else if (strcmp (progname, "gamma") == 0)
    {
      function = Gamma;
    }
  else
    {
      message ("cannot determine desired function from program name\n\tUsing default identity function\n");
      function = identity;
      progname = "identity";
    }


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
      vector_name = "standard input";
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
	      vector_name = argv[optind];
	      do_func (stdin);
	    }
	  optind++;
	}			/* endwhile	 */
    }				/* endif	 */

  end_column (stdout);
  return 0;
}
