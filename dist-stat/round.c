/*
 * $Header: /files1/home/toy/src/stat/RCS/round.c,v 1.2 90/10/25 17:13:51 toy Exp $
 * NAME
 *	round - round the elements of a vector
 *
 * SYNOPSIS
 *	round [-H] [-P prec] [-c cols] [-p p] [-s sigdig] [vectors...]
 *
 * DESCRIPTION
 *	Round the elements as specified.
 *
 * HISTORY
 * $Log:	round.c,v $
 * Revision 1.2  90/10/25  17:13:51  toy
 * Was printing out an extra number at end-of-file.
 *
 * Revision 1.1  90/09/04  18:31:08  toy
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

#define	DEF_PROGNAME	"round"

static const char RCSID[] = "@(#) $Id: round.c,v 1.2 90/10/25 17:13:51 toy Exp $";

extern int getopt ();
extern int optind;
extern char *optarg;

const char *progname;

#define	OPT_STRING	"HP:c:p:s:"

void
help ()
{
  static const char *help_strings[] =
  {
    "\t-H\tThis help\n",
    "\t-c c\tSet number of output elements per line to n.  (Def = 1.)\n",
    "\t-p p\tNumbers are rounded to p places following the decimal\n",
    "\t\tpoint with p is in the range 0 to 9.\n",
    "\t-s s\tNumbers are rounded to s significant digits, with s in\n",
    "\t\tthe range 0 to 9.  The default is 9.\n",
    "Output is the rounded value of each element of the given vectors.\n",
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
	  print_number (stdout, x);
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
  int errcnt;
  int precision;
  double total;

  progname = get_my_name (argv[0], DEF_PROGNAME);

  /*
   * Read options
   */

  precision = 9;
  errcnt = 0;

  while ((option = getopt (argc, argv, OPT_STRING)) != -1)
    {
      switch (option)
	{
	case 'H':		/* Help				 */
	  help ();
	  exit (0);
	  break;
	case 'c':		/* Number of columns		 */
	  (void) set_max_columns (atoi (optarg));
	  break;
	case 'p':		/* Places after decimal point	 */
	  set_f_format ();
	  if (sscanf (optarg, "%d", &precision) != 1)
	    {
	      message ("cannot read -p value:  `%s'\n", optarg);
	      errcnt++;
	    }
	  break;
	case 's':		/* Significant digits		 */
	  if (sscanf (optarg, "%d", &precision) != 1)
	    {
	      message ("cannot read -s value:  `%s'\n", optarg);
	      errcnt++;
	    }
	  break;
	default:
	  break;
	}			/* endswitch	 */
    }				/* endwhile	 */

  /*
   * Check options
   */

  if (errcnt > 0)
    {
      help ();
    }
  if (precision < 0)
    {
      precision = 0;
    }
  else if (precision > 9)
    {
      precision = 9;
    }
  set_precision (precision);


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
