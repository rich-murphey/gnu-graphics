/* plot2ps, a utility for converting Unix plot files into postscript.
   Copyright (C) 1989 Free Software Foundation, Inc.

Plot2ps is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY.  No author or distributor accepts responsibility to anyone for the
consequences of using it or for whether it serves any particular purpose or
works at all, unless he says so in writing.  Refer to the GNU General Public
License for full details.

Everyone is granted permission to copy, modify and redistribute plot2ps, but
only under the conditions described in the GNU General Public License.  A copy
of this license is supposed to have been given to you along with plot2ps so
you can know your rights and responsibilities.  It should be in a file named
COPYING.  Among other things, the copyright notice and this notice must be
preserved on all copies.  */

/* This file is the openpl routine, which is a standard part of the plot
   library.  It normally opens the device.  We just print out the
   postscript prologue */

#include "sys-defines.h"
#include "libplot.h"

extern char *header[6];

/* USER_HAS_PROLOGUE is a flag. If it is non-zero then the open routine
   should output the user specified prologue contained in the file specified
   in the string USERS_PROLOGUE */

int user_has_prologue = 0;

/* USERS_PROLOGUE is a string containing the file name for any user specified
   postscript prologue.  This file is a substitute for the prologue contained
   in the file header.c. */

char *users_prologue = "";

int
openpl ()
{
  int i;

  if (!default_font)
    {
      default_font =  (char *) malloc (strlen ("helvetica") + 1);
      strcpy (default_font, "helvetica");
    }
  if (!current_font)
    {
      current_font =  (char *) malloc (strlen ("helvetica") + 1);
      strcpy (current_font, "helvetica");
    }

  line_type_setdash_length = 1024;
  line_type_setdash = (char *) malloc (line_type_setdash_length);
  if (line_type_setdash <= (char *) 0)
    {
      perror ("malloc failed:");
      exit (-1);
    }

  fputs ("\
%! PS-Adobe-2.0 EPSF-1.2\n\
%%\n\
%%\n\
%%                              TYPE `plot2ps -help' FOR MORE INFORMATION.\n\
%%\n\
%%\n\
%%DocumentFonts: Times-Bold\n\
%%BoundingBox: (atend)", stdout);

  if (user_has_prologue)
    {
      FILE *prologuep;
      char buff[1024];
      int bytes_read;

      prologuep = fopen (users_prologue, "r");
      if (prologuep == NULL)
	{
	  fprintf (stderr, "Unable to open prologue file `%s' for reading. Using builtin prologue.\n", users_prologue);
	  for (i=0; header[i][0]>0; i++)
	    fputs (header[i], stdout);
	}
      else
	{
	  while ( (bytes_read = fread(buff, 1, sizeof(buff), prologuep)) > 0 )
	    {
	      fwrite (buff, 1, bytes_read, stdout);
	    }
	  fclose (prologuep);
	}
    }
  else
    {
      for (i=0; header[i][0]>0; i++)
	fputs (header[i], stdout);
    }

  fputs ("\n\
%I Idraw 7\n\
\n\
Begin\n\
%I b u\n\
%I cfg u\n\
%I cbg u\n\
%I f u\n\
%I p u\n\
%I t\n[ ", stdout);
  printf ("%g 0 0 %g", SCALING, SCALING);
  fputs (" 0 0 ] concat\n\
/originalCTM matrix currentmatrix def\n\
%\n\
%\n\
%\n\
%                              TYPE `plot2ps -help' FOR MORE INFORMATION.\n\
%\n\
%\n\
\n", stdout);

  return 0;
}
