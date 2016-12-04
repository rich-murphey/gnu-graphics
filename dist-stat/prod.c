/*
 * $Header: /files1/home/toy/src/stat/RCS/prod.c,v 1.7 90/09/10 12:23:28 toy Exp $
 * NAME
 *	prod, total - product or sum of the elements of a vector
 *
 * SYNOPSIS
 *	prod [-HT] [-P prec] [vectors...]
 *	total [-HT] [-P prec] [vectors...]
 *
 * DESCRIPTION
 *	Output is the product of the elements of the input
 *	vector(s).
 *
 * HISTORY
 * $Log:	prod.c,v $
 * Revision 1.7  90/09/10  12:23:28  toy
 * Some help strings didn't have a comma in them.
 *
 * Revision 1.6  90/09/05  14:34:38  toy
 * Merged total into this program.
 *
 * Revision 1.5  90/09/04  17:40:39  toy
 * Set default precision from the environment variable STAT_PREC.
 * Use print_help_strings to print out help.
 *
 * Revision 1.4  90/09/01  17:15:04  toy
 * Some more lint removal.
 *
 * Revision 1.3  90/09/01  17:02:22  toy
 * Use message function to print out messages.
 *
 * Revision 1.2  90/09/01  11:26:05  toy
 * Changed option characters.
 *
 * Revision 1.1  90/08/31  15:56:16  toy
 * Initial revision
 *
 */

#include <stdio.h>
#include <string.h>

#if	defined(__STDC__) || defined(__GNUC__)
#include <stddef.h>
#include <stdlib.h>
#endif

#include "gps.h"

#define	DEF_PROGNAME	"prod|total"
#define	PROD_NAME	"prod"
#define	TOTAL_NAME	"total"

static const char RCSID[] = "@(#) $Id: prod.c,v 1.7 90/09/10 12:23:28 toy Exp $";

extern int getopt ();
extern int optind;
extern char *optarg;

const char *progname;

#define	OPT_STRING	"HTP:"

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
    "\t-T\tGrand total of all vectors listed\n",
    "\t\t(otherwise, print result for each vector)\n",
    "\t-P p\tSet number of significant digits to p.  If not specified\n",
    "\t\tuse value of STAT_PREC from environment, if available.  Else\n",
    "\t\tuse default.\n",
    NULL
  };
  static const char *help_prod[] =
  {
    "Compute the product of all the elements of the vector.\n",
    "A product is produced for each specified vector.  If -T is\n",
    "given, the result is the same as if all the vectors were\n",
    "concatenated together.\n",
    NULL
  };
  static const char *help_total[] =
  {
    "Compute the total sum of all the elements of the vector.\n",
    "A total is produced for each specified vector.  If -T is\n",
    "given, the result is the same as if all the vectors were\n",
    "concatenated together.\n",
    NULL
  };
  (void) fprintf (stderr, "%s\n", RCSID);
  (void) fprintf (stderr, "Usage:  %s [-HT] [-P prec] [vector ...]\n", progname);
  print_help_strings (help_strings);
  if (strcmp (progname, PROD_NAME) == 0)
    {
      print_help_strings (help_prod);
    }
  else
    {
      print_help_strings (help_total);
    }
}

#if	defined(__STDC__) || defined(__GNUC__)
double
do_prod (FILE * fp)
#else
double
do_prod (fp)
     FILE *fp;
#endif
{
  int rc;
  double x;
  double prod;

  prod = 1;

  do
    {
      rc = get_number (fp, &x);
      if (rc == 1)
	{
	  prod *= x;
	}
      else if (rc == 0)
	{
	  message ("Error reading number\n");
	}
  } while (rc == 1);

  return prod;
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
  int total_result;
  int product;
  double result;
  double total;
  double (*func) PARMS ((FILE *));

  set_def_prec ();
  progname = get_my_name (argv[0], DEF_PROGNAME);

  /*
   * What's my name?
   */
  if (strcmp (progname, PROD_NAME) == 0)
    {
      func = do_prod;
      product = TRUE;
    }
  else if (strcmp (progname, TOTAL_NAME) == 0)
    {
      func = do_total;
      product = FALSE;
    }
  else
    {
      message ("unknown program name.  Should be prod or total.\n");
      exit (1);
    }

  /*
   * Read options
   */

  total_result = FALSE;
  while ((option = getopt (argc, argv, OPT_STRING)) != -1)
    {
      switch (option)
	{
	case 'H':		/* Help				 */
	  help ();
	  exit (0);
	  break;
	case 'T':		/* Grand total of all vectors	 */
	  total_result = TRUE;
	  break;
	case 'P':		/* Set precision		 */
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
      print_number (stdout, func (stdin));
    }
  else
    {
      total = 1;
      while (optind < argc)
	{
	  if (freopen (argv[optind], "r", stdin) == NULL)
	    {
	      message ("Cannot open `%s'\n", argv[optind]);
	    }
	  else
	    {
	      result = func (stdin);
	      if (total_result)
		{
		  total = product ? total * result : total + result;
		}
	      else
		{
		  print_number (stdout, result);
		}
	    }
	  optind++;
	}			/* endwhile	 */

      if (total_result)
	{
	  print_number (stdout, total);
	}
    }				/* endif	 */

  end_column (stdout);
  return 0;
}
