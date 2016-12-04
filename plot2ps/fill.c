/* This file is the fill routine, which is an extension to the plot library.
   It changes the grey level of the fill patter for all closed of the following
   drawing operations. */

#include "sys-defines.h"
#include "libplot.h"

/* FILL_LEVEL is the intensity of the filler for closed paths.  Intensity
   ranges from 0 to 1. A value of 0 represents black and a value of 1 indicates
   white. A value of -1 represents no fill at all (transparent). */

double fill_level = -1.;

/* FILL sets the intensity of the filler for closed paths.  LEVEL ranges
   from 1 to 0xFFFF. A value of 1 represents black and a value of 0xFFFF
   indicates white. A value of 0 represents no fill - transparent. */

int
fill (level)
     int level;
{
  if (level == 0)
    {
      fill_level = -1.;
    }
  else
    {
      fill_level = ((double)level - 1.)/0xFFFE;
      /* The value of fill level should be between 0 and 1,
	 which is enforced here. */
      if (fill_level > 1.)
	{
	  fill_level = 1.;
	}
      else if (fill_level < 0.)
	{
	  fill_level = 0.;
	}
    }
  return 0;
}
