/* this file is the box routine, which is a standard part of the plot(3)
   library. It draws a box given the coordinates of the diagonal corners, x0,y0
   and x1,y1 */

#include "plot3.h"

int
box (x0, y0, x1, y1)
     int x0, y0, x1, y1;
{
  move (x0, y0);
  cont (x0, y1);
  cont (x1, y1);
  cont (x1, y0);
  cont (x0, y0);
  move (x1, y1);
  return 0;
}
