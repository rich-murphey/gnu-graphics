/* This file is the linemod routine, which is a standard part of the plot
   library.  It sets the line type according to the name contained in
   the string argument s */

#include "sys-defines.h"
#include "libplot.h"

int
linemod (s)
     char *s;
{
  if (strcmp( s, "longdashed") == 0)
    {
      fputs ("\033d", stdout);
    }
  else if (strcmp( s, "disconnected") == 0)
    {
      fputs ("\034", stdout);
    }
  else if (strcmp( s, "dotdashed") == 0)
    {
      fputs ("\033b", stdout);
    }
  else if (strcmp( s, "dotted") == 0)
    {
      fputs ("\033a", stdout);
   }
  else if (strcmp( s, "solid") == 0)
    {
      fputs ("\033`", stdout);
    }
  else if (strcmp( s, "shortdashed") == 0)
    {
      fputs ("\033c", stdout);
    }
  else
    {
      fprintf( stderr, "Unrecognized line mode `%s' ignored.\n", s);	      
    }

  return 0;
}
/* libtek, a library of functions for tektronics 4010 compatible devices.
   Copyright (C) 1989 Free Software Foundation, Inc.

libtek is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY.  No author or distributor accepts responsibility to anyone for the
consequences of using it or for whether it serves any particular purpose or
works at all, unless he says so in writing.  Refer to the GNU General Public
License for full details.

Everyone is granted permission to copy, modify and redistribute libtek, but
only under the conditions described in the GNU General Public License.  A copy
of this license is supposed to have been given to you along with libtek so
you can know your rights and responsibilities.  It should be in a file named
COPYING.  Among other things, the copyright notice and this notice must be
preserved on all copies.  */
