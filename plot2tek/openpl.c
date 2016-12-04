/* Copyright (C) 1989 Free Software Foundation, Inc.

   plot is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY.  No author or distributor accepts responsibility to
   anyone for the consequences of using it or for whether it serves any
   particular purpose or works at all, unless he says so in writing.
   Refer to the GNU General Public License for full details.

   Everyone is granted permission to copy, modify and redistribute
   plot, but only under the conditions described in the GNU General
   Public License.  A copy of this license is supposed to have been
   given to you along with libtek so you can know your rights and
   responsibilities.  It should be in a file named COPYING.  Among
   other things, the copyright notice and this notice must be preserved
   on all copies.  */

/* This file is the openpl routine, which is a standard part of the plot
   library.  It normally opens the graphics device. */

#include "sys-defines.h"
#include "libplot.h"

int user_has_prologue = 0;

struct font_info_struct font_info[NO_OF_FONTS];

int
openpl ()
{
  char* term_type;
  extern char* getenv ();
  term_type = getenv ("TERM");

  if ((term_type != NULL)
      && (strcmp (term_type, "xterm") == 0))
    {
      printf ("\033[?38h");	/* put xterm into Tek mode.
				   "\033\003" switches to VT102 mode. */
    }

  return 0;
}
