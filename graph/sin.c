#include "sys-defines.h"

int
main (argc, argv)
     int argc;
     char **argv;
{
  extern double atof();
  double x, theta, delta_x, x_max;
  theta = atof(argv[1]);
  delta_x = atof(argv[2]);
  x_max = atof(argv[3]);
  for (x = 0.; x < x_max; x += delta_x)
    printf ("%g %g\n", x, sin (theta * x));
  return (0);
}
