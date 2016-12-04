/*
 * $Header: /files1/home/toy/src/stat/RCS/subs.c,v 1.1 90/08/31 14:51:24 toy Exp $
 *
 * $Log:	subs.c,v $
 * Revision 1.1  90/08/31  14:51:24  toy
 * Initial revision
 *
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "gps.h"

#ifndef	lint
static char RCSID[] = "@(#) $Id: subs.c,v 1.1 90/08/31 14:51:24 toy Exp $";
#endif

/*
 * get_my_name
 *
 * Return the name of the current program, or set it to
 * a default name.
 */

#if	defined(__STDC__) || defined(__GNUC__)
const char *
get_my_name (const char *argv, const char *def_name)
#else
char *
get_my_name (argv, def_name)
     char *argv;
     char *def_name;
#endif
{
  const char *name;
  if (argv == NULL)
    {
      name = def_name;
    }
  else
    {
      name = strrchr (argv, '/');
      if (name == NULL)
	{
	  name = argv;
	}
      else
	{
	  name++;
	}
    }

  return name;
}

/*
 * get_number
 *
 * Read a floating point number from the specified file.  Any
 * unknown characters are silently ignored.
 *
 * On return, EOF is returned for the end of file, 0 for some
 * unspecified error, and 1 if the number was read correctly.
 */

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
      ungetc (ch, fp);
      ch = fscanf (fp, "%lf", value);
    }
  return ch;
}

/*
 * print_number
 *
 * Print the specified number to the given file.
 */

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
  fprintf (fp, "%g\n", value);
}
