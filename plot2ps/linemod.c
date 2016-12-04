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
      if (line_type_bit_vector != 0xff00)
	{
	  draw_line ();
	  line_type_bit_vector = 0xff00;
	  sprintf( line_type_setdash, "%d %d", 7, 9);
	  line_type_setdash_bits = 7 + 9 + 1;
	}
    }
  else if (strcmp( s, "disconnected") == 0)
    {
      if (line_type_bit_vector != 0x8000)
	{
	  draw_line ();
	  line_type_bit_vector = 0x8000;
	  sprintf( line_type_setdash, "%d %d", 1, 15);
	  line_type_setdash_bits = 1 + 15 + 1;
	}
    }
  else if (strcmp( s, "dotdashed") == 0)
    {
      if (line_type_bit_vector != 0xffe4)
	{
	  draw_line ();
	  line_type_bit_vector = 0xffe4;
	  sprintf( line_type_setdash, "%d %d %d %d", 11, 2, 1, 2);
	  line_type_setdash_bits =  11 +  2 +  1 +  2 +  1;
	}
    }
  else if (strcmp( s, "dotted") == 0)
    {
      if (line_type_bit_vector != 0x8888)
	{
	  draw_line ();
	  line_type_bit_vector = 0x8888;
	  sprintf( line_type_setdash, "%d %d %d %d", 1, 7, 1, 7);
	  line_type_setdash_bits = 1 + 7 + 1 + 7 + 1;
	}
    }
  else if (strcmp( s, "solid") == 0)
    {
      if (line_type_bit_vector != 0xffff)
	{
	  draw_line ();
	  line_type_bit_vector = 0xffff;
	  sprintf( line_type_setdash, "");
	  line_type_setdash_bits = 0;
	}
    }
  else if (strcmp( s, "shortdashed") == 0)
    {
      if (line_type_bit_vector != 0x0303)
	{
	  draw_line ();
	  line_type_bit_vector = 0x0303;
	  sprintf( line_type_setdash, "%d %d", 2, 14);
	  line_type_setdash_bits = 2 + 14 + 1;
	}
    }
  else
    {
      fprintf( stderr, "Unrecognized line mode `%s' ignored.\n", s);	      
    }

  return 0;
}
