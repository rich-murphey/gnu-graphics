/*
 * $Header: /files1/home/toy/src/stat/RCS/rdvec.c,v 1.2 90/09/10 12:48:32 toy Exp $
 * NAME
 *	read_vector - read a vector into memory
 *
 * SYNOPSIS
 *	double *read_vector(fp, rank)
 *	FILE		*fp;
 *	int		*rank;
 *
 * DESCRIPTION
 *	The numbers given in the file "fp" are read in and
 *	stored in an array.  A pointer to the array is returned.
 *	If no space can be obtained, the program is aborted.
 *
 *	Since malloc is used to allocate space for the array,
 *	the user can free the space returned.  Otherwise memory
 *	will slowly leak away.
 *
 * HISTORY
 * $Log:	rdvec.c,v $
 * Revision 1.2  90/09/10  12:48:32  toy
 * Added RCSID.
 *
 * Revision 1.1  90/09/02  17:13:48  toy
 * Initial revision
 *
 */

#include <stdio.h>

#if	defined(__STDC__) || defined(__GNUC__)
#include <stddef.h>
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#include "gps.h"

#ifndef	lint
static char RCSID[] = "@(#) $Id: rdvec.c,v 1.2 90/09/10 12:48:32 toy Exp $";
#endif

#define	DEF_VEC_SIZE	1000
#define	DEF_VEC_INC	1000

#if	defined(__STDC__) || defined(__GNUC__)
double *
read_vector (FILE * fp, int *rank)
#else
double *
read_vector (fp, rank)
     FILE *fp;
     int *rank;
#endif
{
  int rc;
  double x;
  double *vec;
  int max_alloc;

  /*
   * Allocate some space for the vector
   */
  max_alloc = DEF_VEC_SIZE;
  vec = malloc (max_alloc * sizeof (double));
  if (vec == NULL)
    {
      message ("cannot get space for %d elements\n", max_alloc);
      abort ();
    }
  *rank = 0;
  do
    {
      rc = get_number (fp, &x);
      if (rc == 1)
	{

	  /*
           * Get more space to hold the number if we need
           * it
           */
	  if (*rank >= max_alloc)
	    {
	      max_alloc += DEF_VEC_INC;
	      vec = realloc (vec, max_alloc * sizeof (double));
	      if (vec == NULL)
		{
		  message ("cannot get space for %d elements\n", max_alloc);
		  abort ();
		}
	    }
	  vec[(*rank)++] = x;
	}
      else if (rc == 0)
	{
	  message ("Error reading number\n");
	}
  } while (rc == 1);

  return vec;
}
