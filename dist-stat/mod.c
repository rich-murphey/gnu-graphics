/*
 * $Header: /files1/home/toy/src/stat/RCS/mod.c,v 1.2 90/09/04 17:40:35 toy Exp $
 * NAME
 *	mod - modulo function for a vector
 *
 * SYNOPSIS
 *	cusum [-H] [-P prec] [-c cols] [-m mod] [vectors...]
 *
 * DESCRIPTION
 *	Output is the modulus of the elements of
 *	the specified vector(s).
 *
 * HISTORY
 * $Log:	mod.c,v $
 * Revision 1.2  90/09/04  17:40:35  toy
 * Set default precision from the environment variable STAT_PREC.
 * Use print_help_strings to print out help.
 *
 * Revision 1.1  90/09/01  17:23:08  toy
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

#define	DEF_PROGNAME	"mod"

static const char RCSID[] = "@(#) $Id: mod.c,v 1.2 90/09/04 17:40:35 toy Exp $";

extern int getopt ();
extern int optind;
extern char *optarg;

const char *progname;

#define	OPT_STRING	"HP:c:m:"

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
    "\t-m m\tSet modulus to m.  Default is 2.\n",
    "Compute the modulus of each element of the specified vectors.\n",
    "See fmod(3) for exact details.\n",
    NULL
  };
  (void) fprintf (stderr, "%s\n", RCSID);
  (void) fprintf (stderr, "Usage:  %s [-H] [-P prec] [-c cols] [vector ...]\n", progname);
  print_help_strings (help_strings);
}



#if	defined(__STDC__) || defined(__GNUC__)
void
do_func (FILE * fp, double modulus)
#else
void
do_func (fp, modulus)
     FILE *fp;
     double modulus;
#endif
{
  int rc;
  double x;

  do
    {
      rc = get_number (fp, &x);
      if (rc == 1)
	{
	  print_number (stdout, fmod (x, modulus));
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
  double modulus;

  progname = get_my_name (argv[0], DEF_PROGNAME);
  set_def_prec ();


  /*
   * Read options
   */

  modulus = 2;

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
	case 'm':		/* Modulus			 */
	  modulus = atof (optarg);
	  break;
	default:
	  break;
	}			/* endswitch	 */
    }				/* endwhile	 */

  if (modulus == 0)
    {
      message ("modulus cannot be zero.\n");
      exit (1);
    }

  /*
   * If no files are listed, use stdin.  Otherwise use
   * the given files
   */

  if (optind == argc)
    {
      do_func (stdin, modulus);
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
	      do_func (stdin, modulus);
	    }
	  optind++;
	}			/* endwhile	 */
    }				/* endif	 */

  end_column (stdout);
  return 0;
}
