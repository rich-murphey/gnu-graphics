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

/* FONTNAME takes a string argument S containing the name of the desired current
   font and sets the current font to that name.  If the string S is too long
   it complains and sets the current font to the default. */

#include "sys-defines.h"
#include "libplot.h"

int
fontname (s)
    char *s;
{
  if (strlen (s) >= strlen(current_font))
    current_font =  (char *) realloc (current_font, strlen (s) + 1);

  (void) strcpy (current_font, s);
  return 0;
}
