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

/* This file is the erase routine, which is a standard part of the plot
   library. It erases the page. We don't do anything since we generally plot one
   page at a time and will usually find an erase at the head of a plot file.
   */

#include "sys-defines.h"
#include "libplot.h"

int
erase ()
{
  draw_line ();
  return 0;
}
