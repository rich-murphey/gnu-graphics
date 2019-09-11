/* plot, unix plot file to graphics device translators.
   Copyright (C) 1989 Free Software Foundation, Inc.

   plot is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY.  No author or distributor accepts responsibility to
   anyone for the consequences of using it or for whether it serves any
   particular purpose or works at all, unless he says so in writing.
   Refer to the GNU General Public License for full details.
   
   Everyone is granted permission to copy, modify and redistribute plot,
   but only under the conditions described in the GNU General Public
   License.  A copy of this license is supposed to have been given to you
   along with plot so you can know your rights and responsibilities.  It
   should be in a file named COPYING.  Among other things, the copyright
   notice and this notice must be preserved on all copies.  */


/* FONTNAME takes a string argument S containing the name of the
   desired current font and sets the current font to that name. */

#include "sys-defines.h"
#define  _REGEX_RE_COMP
#include <regex.h>
#include "libplot.h"
#include "extern.h"

/* a simple fixed string matcher. match returns
   1 if there is a match for s1 in s2, else 0. */
int
match (s1, s2)
     char *s1, *s2;
{
  int len2, i;
  len2 = strlen (s2);

  for (i=0; i<len2; i++)
    {
      if (strcmp (s1, &s2[i]) == 0)
	{
	  return 1;
	}
    }
  return 0;
}

int font_id = 0; /* the default font */

int
fontname (s)
    char *s;
{
  char *res;

  res = (char *) re_comp (s);
  if (res)
    {
      fprintf (stderr, "error in matching fontname: %s\n", res);
      fprintf (stderr, "Fontname `%s' ignored.\n", s);
      return 0;
    }

  if (re_exec("typewriter")
      || re_exec("courier"))
    font_id = 5;
  else if (re_exec("modern"))
    font_id = 4;
  else if (re_exec("italic")
	   || re_exec("oblique"))
    font_id = 3;
  else if (re_exec("bold"))
    font_id = 2;
  else if (re_exec("times")
      || re_exec("roman"))
    font_id = 2;
  else
    fprintf (stderr, "Unrecognized font name `%s' ignored.\n", s);

  return 0;
}

