/*
 * $Header: /files1/home/toy/src/stat/RCS/message.c,v 1.2 90/09/10 12:48:23 toy Exp $
 * NAME
 *	errmsg - print a message to stderr
 *
 * SYNOPSIS
 *	void errmsg(format, ...)
 *	char	*format;
 *
 * DESCRIPTION
 *	Print a message to stderr with the name of the program
 *	prepended.
 *
 * HISTORY
 * $Log:	message.c,v $
 * Revision 1.2  90/09/10  12:48:23  toy
 * Added RCSID.
 *
 * Revision 1.1  90/09/01  17:02:00  toy
 * Initial revision
 *
 */

#include <stdio.h>
#include "config.h"
extern char *progname;

#ifndef	lint
static char RCSID[] = "@(#) $Id: message.c,v 1.2 90/09/10 12:48:23 toy Exp $";
#endif

#ifdef STDC_HEADERS 

#include <stdarg.h>
void
message (char *fmt, ...)
{
  va_list argp;
  fprintf (stderr, "%s:  ", progname);
  va_start(argp, fmt);
  vfprintf(stderr, fmt, argp);
  va_end(argp);
}

#else
#include <varargs.h>
void
message (va_alist)
     va_dcl
{
  char *format;
  va_list argp;

  va_start (argp);
  format = va_arg (argp, char *);

  (void) fprintf (stderr, "%s:  ", progname);
  (void) vfprintf (stderr, format, argp);
}

#endif
