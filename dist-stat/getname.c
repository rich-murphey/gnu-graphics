/*
 * $Header: /files1/home/toy/src/stat/RCS/getname.c,v 1.2 90/09/02 11:15:50 toy Exp $
 * NAME
 *	get_my_name
 *
 * SYNOPSIS
 *	const char *get_my_name(argv, def_name)
 *	const char	*argv;
 *	const char	*def_name;
 *
 * DESCRIPTION
 *	Return the name of the current program, or set it to
 *	a default name.  "argv" should be set to argv[0] in the
 *	main program.  If argv is NULL, then the default name
 *	"def_name" is used.
 *
 * HISTORY
 * $Log:	getname.c,v $
 * Revision 1.2  90/09/02  11:15:50  toy
 * Added support for MSDOS
 *
 * Revision 1.1  90/09/01  11:35:03  toy
 * Initial revision
 *
 */

#if	defined(__STDC__) || defined(__GNUC__)
#include <stddef.h>
#include <string.h>
#else
#include <stdio.h>
#include <string.h>
#endif

#ifdef	BSD
#define	strrchr(s, c)	index(s, c)
#endif

#ifndef	lint
static char RCSID[] = "@(#) $Id: getname.c,v 1.2 90/09/02 11:15:50 toy Exp $";
#endif

#if	defined(__TURBOC__) || defined(MSDOS)
#define	PATH_SEP	'\\'
#include <stdlib.h>
extern char *strlwr (char *s);
#else
#define	PATH_SEP	'/'
#endif

#if	defined(__STDC__) || defined(__GNUC__)
char *
get_my_name (char *argv, const char *def_name)
#else
char *
get_my_name (argv, def_name)
     char *argv;		/* argv[0] in main	 */
     char *def_name;		/* Default name		 */
#endif
{
  char *name;

  if (argv == NULL)
    {
      name = (def_name != NULL) ? def_name : "Unknown??";
    }
  else
    {
      name = strrchr (argv, PATH_SEP);
      if (name == NULL)
	{
	  name = argv;
	}
      else
	{
	  name++;
	}
#if	defined(__TURBOC__) || defined(MSDOS)
      {
	char *name_buf;
	name_buf = malloc (strlen (name) + 1);
	strcpy (name_buf, name);
	*strrchr (name_buf, '.') = '\0';
	(void) strlwr (name_buf);
	strcpy (argv, name_buf);
	free (name_buf);
      }
#endif
    }

  return name;
}
