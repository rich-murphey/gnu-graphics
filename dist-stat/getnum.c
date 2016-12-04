/*
 * $Header: /files1/home/toy/src/stat/RCS/getnum.c,v 1.1 90/09/01 11:36:04 toy Exp $
 * NAME
 *	get_number - read a number, skipping over invalid characters
 *
 * SYNOPSIS
 *	int get_number(fp, value)
 *	FILE	*fp;
 *	double	*value;
 *
 * DESCRIPTION
 *	Read a floating point number from the specified file.  Any
 *	unknown characters are silently ignored.
 *
 *	On return, EOF is returned for the end of file, 0 for some
 *	unspecified error, and 1 if the number was read correctly.
 */

#include <stdio.h>
#include <ctype.h>

#ifndef	lint
static char RCSID[] = "@(#) $Id: getnum.c,v 1.1 90/09/01 11:36:04 toy Exp $";
#endif

#if	defined(__STDC__) || defined(__GNUC__)
int
get_number (FILE * fp, double *value)
#else
int
get_number (fp, value)
     FILE *fp;
     double *value;
#endif
{
  int ch;

  /*
   * Skip over any characters that can't be part of a
   * number
   */

  do
    {
      ch = getc (fp);
  } while ((ch != EOF) && (ch != '-') && (ch != '.') && (!isdigit (ch)));

  if (ch != EOF)
    {

      /*
       * We must have something that looks like the start of a number,
       * so pretend it is.  If it's not, fscanf will tell us that.
       */

      (void) ungetc (ch, fp);
      ch = fscanf (fp, "%lf", value);
    }
  return ch;
}
