/*
 * $Header: /files1/home/toy/src/stat/RCS/cor.c,v 1.5 90/09/11 16:45:19 toy Exp $
 * NAME
 *	cor - ordinary correlation coefficient
 *
 * SYNOPSIS
 *	cor [-H] [-P prec] [-c cols] [-F base] [vectors...]
 *	lreg [-H] [-P prec] [-c cols] [-F base] [-ios] [vectors...]
 *
 * DESCRIPTION
 *	Output for cor is the ordinary correlation coefficient between
 *	the base vector and the given vectors.  If the base is not given,
 *	it is assumed to come from stdin.
 *
 *	The correlation is:
 *
 *		E[(x - mx)(y - my)]
 *		-------------------
 *		sqrt(Var x * Var y)
 *
 *	where mx, and my are the means.
 *
 *	For lreg, the output is the slope and intercept from a least
 *	squares linear regression of each vector on a base vector.
 *	If the base is not given, it is assumed to be ascending non-
 *	negative integers from 0.
 *
 * BUGS
 *	The entire vector is read into memory to compute the variance.
 *	This is done for accuracy (but is it really necessary?).
 *
 * HISTORY
 * $Log:	cor.c,v $
 * Revision 1.5  90/09/11  16:45:19  toy
 * If vector is shorter than the base vector, the message should
 * include the file name and the number of elements read.
 *
 * Also recognize "," as an option character that does nothing.
 *
 * Revision 1.4  90/09/05  13:53:54  toy
 * Implemented lreg routine here.
 *
 * Revision 1.3  90/09/04  16:04:03  toy
 * Use print_help_strings to print out the help strings.
 * Get the default precision from the environment variable STAT_PREC.
 *
 * Revision 1.2  90/09/04  14:53:57  toy
 * The correlation coefficient should be computed with
 * the appropriate means subtracted out.
 *
 * Revision 1.1  90/09/02  19:01:47  toy
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

#define	DEF_PROGNAME	"cor|lreg"
#define	COR_NAME	"cor"
#define	LREG_NAME	"lreg"

static const char RCSID[] = "@(#) $Id: cor.c,v 1.5 90/09/11 16:45:19 toy Exp $";

extern int getopt ();
extern int optind;
extern char *optarg;

const char *progname;
const char *vector_name;

int intercept_output = TRUE;	/* lreg:  output intercept	 */
int slope_output = TRUE;	/* lreg:  output slope		 */
int opt_form = FALSE;		/* lreg:  output in option form	 */

#define	COR_OPT_STRING	",HP:c:F:"
#define	LREG_OPT_STRING	",HP:c:F:ios"

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
    "\t-F F\tBase (x) vector is read from file F.  If not specified, the\n",
    "\t\tdata is read from stdin.\n",
    NULL
  };
  static const char *help_cor[] =
  {
    "\tvec..\tData vectors.  If not given, use stdin.\n",
    "Compute and print the correlation coefficient between the base vector and\n",
    "the given data vectors.  (The coefficient is computed with the mean removed.)\n",
    NULL
  };
  static const char *help_lreg[] =
  {
    "\t-i\tOutput intercept only\n",
    "\t-s\tOutput slope only\n",
    "\t-o\tOutput result in option form (for siline)\n",
    "Compute the slope and intercept of a least-squares linear regression\n",
    "between the base (x) vector and the give data (y) vectors.\n",
    "The intercept and then slope are printed.\n",
    NULL
  };

  (void) fprintf (stderr, "%s\n", RCSID);

  if (strcmp (progname, COR_NAME) == 0)
    {
      (void) fprintf (stderr, "Usage:  %s [-H] [-P prec] [-c cols] [-F base] [vector ...]\n", progname);
      print_help_strings (help_strings);
      print_help_strings (help_cor);
    }
  else
    {
      (void) fprintf (stderr, "Usage:  %s [-Hios] [-P prec] [-c cols] [-F base] [vector ...]\n", progname);
      print_help_strings (help_strings);
      print_help_strings (help_lreg);
    }
}


#if	defined(__STDC__) || defined(__GNUC__)
void
do_cor (FILE * fp, double *base_vector, double base_mean, double base_var, int base_rank)
#else
void
do_cor (fp, base_vector, base_mean, base_var, base_rank)
     FILE *fp;
     double *base_vector;
     double base_mean;
     double base_var;
     int base_rank;
#endif
{
  int n;
  int k;
  double cross;
  double var;
  double mean;
  double slope;
  double intercept;
  double diff;
  double *vector;
  double *ptr;

  /*
   * Read in the vector
   */

  vector = read_vector (fp, &n);

  if (n < base_rank)
    {
      message ("insufficient number of elements (%d of %d) in vector `%s'\n",
	       n, base_rank, vector_name);
    }
  else
    {

      /*
       * compute the cross-correlation and variance of
       * both the data vector
       */
      mean = cross = var = 0;
      ptr = vector;
      for (k = 0; k < base_rank; k++)
	{
	  mean += *ptr++;
	}
      mean /= base_rank;

      ptr = vector;
      for (k = 0; k < base_rank; k++)
	{
	  diff = *ptr++ - mean;
	  cross += (*base_vector++ - base_mean) * diff;
	  var += diff * diff;
	}

      if (strcmp (progname, COR_NAME) == 0)
	{

	  /*
           * Compute correlation and print it out
           */
	  slope = cross / sqrt (base_rank * base_var * var);
	  print_number (stdout, slope);
	}
      else
	{

	  /*
           * Compute the slope and intercept
           */
	  slope = cross / base_var / base_rank;
	  intercept = mean - slope * base_mean;
	  if (opt_form)
	    {
	      int prec;
	      prec = set_precision (-1);	/* Get current precision	 */
	      if (intercept_output)
		{
		  printf ("-i%.*g ", prec, intercept);
		}
	      if (slope_output)
		{
		  printf ("-s%.*g ", prec, slope);
		}
	    }
	  else
	    {
	      if (intercept_output)
		{
		  print_number (stdout, intercept);
		}
	      if (slope_output)
		{
		  print_number (stdout, slope);
		}
	    }
	}
    }

  free (vector);
}

/*
 * Compute the mean and variance of the given vector
 */

#if	defined(__STDC__) || defined(__GNUC__)
void
calc_base_var (double *mean, double *var, double *base, int cnt)
#else
void
calc_base_var (mean, var, base, cnt)
     double *mean;
     double *var;
     double *base;
     int cnt;
#endif
{
  int k;
  double diff;
  double *base_ptr;

  *var = 0;
  *mean = 0;
  base_ptr = base;

  for (k = 0; k < cnt; k++)
    {
      *mean += *base_ptr++;
    }
  *mean /= cnt;

  for (k = 0; k < cnt; k++)
    {
      diff = *base++ - *mean;
      *var += diff * diff;
    }
  *var /= cnt;
}

int
main (argc, argv)
     int argc;
     char *argv[];
{
  char *base;
  const char *opt_string;
  int option;
  int errcnt;
  int base_rank;
  double base_mean;
  double base_var;
  double *base_vector;

  set_def_prec ();
  progname = get_my_name (argv[0], DEF_PROGNAME);

  /*
   * Do I know who I am?
   */

  if (strcmp (progname, COR_NAME) == 0)
    {
      opt_string = COR_OPT_STRING;
      (void) set_max_columns (1);
    }
  else if (strcmp (progname, LREG_NAME) == 0)
    {
      opt_string = LREG_OPT_STRING;
      (void) set_max_columns (2);
    }
  else
    {
      message ("unknown program name.  Should be cor or lreg.\n");
      exit (1);
    }

  errcnt = 0;
  base = NULL;

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
	case 'c':		/* Number of output columns	 */
	  (void) set_max_columns (atoi (optarg));
	  break;
	case 'F':		/* Base vector file		 */
	  base = optarg;
	  break;
	case 'i':		/* lreg:  intercept only	 */
	  slope_output = FALSE;
	  break;
	case 'o':		/* lreg:  output in option form	 */
	  opt_form = TRUE;
	  break;
	case 's':		/* lreg:  slope only		 */
	  intercept_output = FALSE;
	  break;
	case ',':
	default:
	  break;
	}			/* endswitch	 */
    }				/* endwhile	 */

  /*
   * Check options
   */

  if ((base == NULL) && (optind >= argc))
    {
      message ("the base vector and argument vectors cannot both be from stdin\n");
      errcnt++;
    }

  /*
   * If a base vector is given, read it from there.
   * Otherwise read it from stdin.
   */

  if (base == NULL)
    {
      base_vector = read_vector (stdin, &base_rank);
    }
  else
    {
      FILE *fp;
      fp = fopen (base, "r");
      if (fp == NULL)
	{
	  message ("cannot open base file `%s'\n", base);
	  errcnt++;
	}
      else
	{
	  base_vector = read_vector (fp, &base_rank);
	  (void) fclose (fp);
	}
    }

  /*
   * Exit on errors
   */

  if (errcnt > 0)
    {
      exit (1);
    }
  calc_base_var (&base_mean, &base_var, base_vector, base_rank);

  /*
   * Now correlate the base vector with the given vectors
   * or stdin, as appropriate.
   */

  if (optind >= argc)
    {
      vector_name = "standard input";
      do_cor (stdin, base_vector, base_mean, base_var, base_rank);
    }
  else
    {
      FILE *fp;
      for (; optind < argc; optind++)
	{
	  fp = fopen (argv[optind], "r");
	  if (fp == NULL)
	    {
	      message ("cannot open vector `%s'\n", argv[optind]);
	    }
	  else
	    {
	      vector_name = argv[optind];
	      do_cor (fp, base_vector, base_mean, base_var, base_rank);
	    }
	}			/* endfor	 */
    }

  end_column (stdout);
  return 0;
}
