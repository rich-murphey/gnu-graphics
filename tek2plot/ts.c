#include "sys-defines.h"
#include "plot3.h"

int
main (argc, argv)
     int argc;
     char *argv[];
{
  int x,y,r;
  openpl();
  erase ();

  fill (0xFFFF);
  move (20,20);
  cont (20,700);
  cont (700,700);
  cont (700,20);
  cont (20,20);
  move (100,100);
  fill (0);

  label ("hi there ");
  label ("xxxxxxxx");

  circle (300,300,200);

  x=200; y=300; r=40;
  arc (x,y,x,y+r,x-r,y);
  

  closepl();
  return 0;
}
