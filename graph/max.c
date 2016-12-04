#include "sys-defines.h"
#include "extern.h"
#ifndef MAXFLOAT
#define MAXFLOAT 1e29
#endif

void
array_min (p, length, min_x, min_y)
     point_struct *p;
     int length;
     double *min_x;
     double *min_y;
{
  int i;

  if (length == 0)
    {
      *min_x = 0.;
      *min_y = 0.;
      return;
    }
  *min_x = MAXFLOAT;
  *min_y = MAXFLOAT;
    
  for (i=0; i<length; i++)
    {
      if (p[i].x < *min_x)
	*min_x = p[i].x;
      if (p[i].y < *min_y)
	*min_y = p[i].y;
    }
}


void
array_max (p, length, max_x, max_y)
     point_struct *p;
     int length;
     double *max_x;
     double *max_y;
{
  int i;

  if (length == 0)
    {
      *max_x = 0.;
      *max_y = 0.;
      return;
    }
  *max_x = -1. * MAXFLOAT;
  *max_y = -1. * MAXFLOAT;
    
  for (i=0; i<length; i++)
    {
      if (p[i].x > *max_x)
	*max_x = p[i].x;
      if (p[i].y > *max_y)
	*max_y = p[i].y;
    }
}
