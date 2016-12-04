/*
 * $Header: /files1/home/toy/src/stat/RCS/power.c,v 1.3 90/11/04 18:18:21 toy Exp $
 * NAME
 *	power, root - raise vector to a power, or take the appropriate root
 *
 * SYNOPSIS
 *	power [-H] [-P prec] [-c cols] [-p power] [vectors...]
 *	root [-H] [-P prec] [-c cols] [-r root] [vectors...]
 *
 * DESCRIPTION
 *	Output is the modulus of the elements of
 *	the specified vector(s).
 *
 * HISTORY
 * $Log:	power.c,v $
 * Revision 1.3  90/11/04  18:18:21  toy
 * Fixed help message.
 *
 * Revision 1.2  90/09/04  17:40:37  toy
 * Set default precision from the environment variable STAT_PREC.
 * Use print_help_strings to print out help.
 *
 * Revision 1.1  90/09/01  17:42:43  toy
 * Initial revision
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

#define	DEF_PROGNAME	"[power|root]"

static const char RCSID[] = "@(#) $Id: power.c,v 1.3 90/11/04 18:18:21 toy Exp $";

extern int getopt ();
extern int optind;
extern char *optarg;

const char *progname;

#define	POWER_OPT_STRING	"HP:c:p:"
#define	ROOT_OPT_STRING		"HP:c:r:"

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
    NULL
  };

  static const char *root_help[] =
  {
    "\t-r r\tSet root to r.  Default is 2 (square root).\n",
    "Compute the r'th root of the elements of the vector(s).\n",
    NULL
  };

  static const char *power_help[] =
  {
    "\t-p p\tSet power to p.  Default is 2 (square).\n",
    "Compute the p'th power of the elements of the vector(s).\n",
    NULL
  };

  (void) fprintf (stderr, "%s\n", RCSID);
  (void) fprintf (stderr, "Usage:  %s [-H] [-P prec] [-c cols] [-p power] [vector ...]\n", progname);
  print_help_strings (help_strings);

  if (strcmp (progname, "root") == 0)
    {
      print_help_strings (root_help);
    }
  else
    {
      print_help_strings (power_help);
    }
}



#if	defined(__STDC__) || defined(__GNUC__)
void
do_func (FILE * fp, double power)
#else
void
do_func (fp, power)
     FILE *fp;
     double power;
#endif
{
  int rc;
  double x;

  do
    {
      rc = get_number (fp, &x);
      if (rc == 1)
	{
	  print_number (stdout, pow (x, power));
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
  const char *opt_string;
  int option;
  double power;

  progname = get_my_name (argv[0], DEF_PROGNAME);
  set_def_prec ();

  /*
   * Figure out what to do based on our name.  The
   * default is power.
   */

  if (strcmp (progname, "power") == 0)
    {
      power = 2;
      opt_string = POWER_OPT_STRING;
    }
  else if (strcmp (progname, "root") == 0)
    {
      power = 0.5;
      opt_string = ROOT_OPT_STRING;
    }
  else
    {
      message ("cannot determine desired function from program name.\n\tUsing default of power\n");
      power = 2;
      opt_string = POWER_OPT_STRING;
      progname = DEF_PROGNAME;
    }


  /*
   * Read options
   */

  while ((option = getopt (argc, argv, opt_string)) != -1)
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
	case 'p':		/* Power			 */
	  power = atof (optarg);
	  break;
	case 'r':		/* Root				 */
	  power = atof (optarg);
	  if (power == 0)
	    {
	      message ("root cannot be zero\n");
	      exit (1);
	    }
	  power = 1 / power;
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
      do_func (stdin, power);
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
	      do_func (stdin, power);
	    }
	  optind++;
	}			/* endwhile	 */
    }				/* endif	 */

  end_column (stdout);
  return 0;
}
