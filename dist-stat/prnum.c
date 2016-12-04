/*
 * $Header: /files1/home/toy/src/stat/RCS/prnum.c,v 1.5 90/09/10 19:03:03 toy Exp $
 * NAME
 *	print_number, set_max_columns, set_precision, end_column - print out numbers in columns
 *
 * SYNOPSIS
 *	void print_number(fp, value)
 *	FILE	*fp;
 *	double	value;
 *
 *	int set_max_columns(max)
 *	int	max;
 *
 *	int set_precision(prec)
 *	int	prec;
 *
 *	void set_f_format()
 *
 *	void set_g_format()
 *
 *	void end_column(fp)
 *	FILE	*fp;
 *
 *	void next_column()
 *
 * DESCRIPTION
 *	A table of numbers is printed out in neat columns.  The
 *	routine "print_number" prints "value" to the file "fp" and
 *	places newlines in the correct place.
 *
 *	The number of columns to be printed is set by the routine
 *	set_max_columns.  If a non-positive number is set, the
 *	default of 5 is used.  The previous value of the number
 *	of columns is returned.
 *
 *	The precision of the numbers printed is controlled by the
 *	routine "set_precision".  The precision is the number of
 *	significant digits in the printed result.  If a non-
 *	positive number value for "prec" is specified, the
 *	default is used.  The previous precision is returned.
 *
 *	To set the format of the numbers, use set_f_format or
 *	set_g_format, which sets the format to f or g, respectively.
 *	(See printf(3) for an explanation).  The default is g format.
 *
 *	To neatly terminate the columnar output, use the
 *	routine "end_column" to print a newline character,
 *	if needed.
 *
 *	The routine "next_column" advances the column counter
 *	to the next column and prints out the appropriate
 *	column separator (either a tab or a newline).
 *
 * BUGS
 *	The routines are not reentrant so if printing is done
 *	to more than one file at a time, the columns are wrong.
 *
 * HISTORY
 * $Log:	prnum.c,v $
 * Revision 1.5  90/09/10  19:03:03  toy
 * Added next_column routine.
 *
 * Revision 1.4  90/09/10  12:23:04  toy
 * Need to included gps.h.
 *
 * Revision 1.3  90/09/04  18:31:27  toy
 * Added routines to allow printing numbers in f or g format.
 *
 * Revision 1.2  90/09/02  11:16:06  toy
 * More intelligent printing of numbers by tabs and
 * newlines at the appropriate places.
 *
 * Revision 1.1  90/09/01  11:35:18  toy
 * Initial revision
 *
 */

#include <stdio.h>

#include "gps.h"

#ifndef	lint
static char RCSID[] = "@(#) $Id: prnum.c,v 1.5 90/09/10 19:03:03 toy Exp $";
#endif

#define	DEF_PRECISION	6
#define	DEF_MAX_COLUMNS	5
#define	DEF_FORMAT	"%.*g"

static const char *format = DEF_FORMAT;
static int max_columns = DEF_MAX_COLUMNS;
static int precision = DEF_PRECISION;
static int column = 0;

#if	defined(__STDC__) || defined(__GNUC__)
int
set_max_columns (int max)
#else
int
set_max_columns (max)
     int max;
#endif
{
  int old_max;

  old_max = max_columns;
  max_columns = (max <= 0) ? DEF_MAX_COLUMNS : max;

  return old_max;
}

#if	defined(__STDC__) || defined(__GNUC__)
int
set_precision (int prec)
#else
int
set_precision (prec)
     int prec;
#endif
{
  int old_prec;

  old_prec = precision;
  precision = (prec <= 0) ? DEF_PRECISION : prec;

  return old_prec;
}

#if	defined(__STDC__) || defined(__GNUC__)
void
print_number (FILE * fp, double value)
#else
void
print_number (fp, value)
     FILE *fp;
     double value;
#endif
{
  (void) fprintf (fp, format, precision, value);

  if (++column >= max_columns)
    {
      putc ('\n', fp);
      column = 0;
    }
  else
    {
      putc ('\t', fp);
    }
}

#if	defined(__STDC__) || defined(__GNUC__)
void
set_f_format (void)
#else
void
set_f_format ()
#endif
{
  format = "%.*f";
}

#if	defined(__STDC__) || defined(__GNUC__)
void
set_g_format (void)
#else
void
set_g_format ()
#endif
{
  format = DEF_FORMAT;
}

#if	defined(__STDC__) || defined(__GNUC__)
void
end_column (FILE * fp)
#else
void
end_column (fp)
     FILE *fp;
#endif
{
  if (column != 0)
    {
      putc ('\n', fp);
    }
  column = 0;
}


#if	defined(__STDC__) || defined(__GNUC__)
void
next_column (FILE * fp)
#else
void
next_column (fp)
     FILE *fp;
#endif
{
  if (++column >= max_columns)
    {
      putc ('\n', fp);
      column = 0;
    }
  else
    {
      putc ('\t', fp);
    }
}
