/*
 * $Header: /files1/home/toy/src/stat/RCS/setprec.c,v 1.1 90/09/04 16:03:35 toy Exp $
 *
 * NAME
 *	set_def_prec - set default number precision
 *
 * SYNOPSIS
 *	set_def_prec()
 *
 * DESCRIPTION
 *	The default precision for printing numbers is set by
 *	reading the environment variable STAT_PREC.  This should
 *	be set to an integer which gives the number of significant
 *	digits that should be used for printing numbers.
 *
 * HISTORY
 * $Log:	setprec.c,v $
 * Revision 1.1  90/09/04  16:03:35  toy
 * Initial revision
 *
 */

#include <stdio.h>

#if	defined(__STDC__) || defined(__GNUC__)
#include <stddef.h>
#include <stdlib.h>
#else
extern char *getenv ();
#endif

#include "gps.h"

#ifndef	lint
static char RCSID[] = "@(#) $Id: setprec.c,v 1.1 90/09/04 16:03:35 toy Exp $";
#endif

#if	defined(__STDC__) || defined(__GNUC__)
void
set_def_prec (void)
#else
void
set_def_prec ()
#endif
{
  char *prec_str;
  int prec;

  prec_str = getenv ("STAT_PREC");
  if ((prec_str != NULL) && (sscanf (prec_str, "%d", &prec) == 1))
    {
      set_precision (prec);
    }
}
