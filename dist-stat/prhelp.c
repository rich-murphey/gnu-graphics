/*
 * $Header: /files1/home/toy/src/stat/RCS/prhelp.c,v 1.2 90/09/10 12:48:30 toy Exp $
 * NAME
 *	print_help_strings - print help strings to stderr
 *
 * SYNOPSIS
 *	print_help_strings(str_array)
 *	char	*str_array[];
 *
 * DESCRIPTION
 *	Print the array of strings to stderr.  The array
 *	must end with a NULL pointer (as opposed to the
 *	null string "").
 *
 * HISTORY
 * $Log:	prhelp.c,v $
 * Revision 1.2  90/09/10  12:48:30  toy
 * Added RCSID.
 *
 * Revision 1.1  90/09/04  16:03:24  toy
 * Initial revision
 *
 */

#include <stdio.h>

#ifndef	lint
static char RCSID[] = "@(#) $Id: prhelp.c,v 1.2 90/09/10 12:48:30 toy Exp $";
#endif

#if	defined(__STDC__) || defined(__GNUC__)
void
print_help_strings (const char *str_array[])
#else
void
print_help_strings (str_array)
     char *str_array[];
#endif
{
  while (*str_array != NULL)
    {
      fputs (*str_array++, stderr);
    }
}
