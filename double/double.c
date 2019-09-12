/* double - filter for converting, scaling and cutting unformatted (binary) or
   ascii double precision data streams.

   Copyright (C) 1989 Free Software Foundation, Inc.

   Plot is distributed in the hope that it will be useful, but WITHOUT
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

#include "sys-defines.h"
#include <stdio.h>
#include <sys/file.h>
#include <ctype.h>
#include <math.h>

char *help_message="\n\
double - a utility for converting, scaling, cutting and pasting unformatted\n\
 (binary) or ascii double precision data.\n\
\n\
Usage:                 double [-option [ARG]] ...<DATA\n\
\n\
Option:         Description:\n\
\n\
add FILE        add values from FILE to each value from DATA.\n\
baseline VALUE  add VALUE to each value from DATA (default 0.0).\n\
bi              read binary (unformatted double precision) values from DATA.\n\
bo              output binary (unformatted double precision) data.\n\
fo              output binary (unformatted single precision) data.\n\
copy NUMBER     output NUMBER values before skipping (default 1).\n\
h               print this help message\n\
join FILE       output one value from FILE for each value from DATA.\n\
mult FILE       multiply values from FILE with each value from DATA.\n\
max             print out the maximum data value.\n\
min             print out the minimum data value.\n\
offset NUMBER   ignore NUMBER values before reading value from DATA.\n\
scale VALUE     scale every output datum by VALUE (default 1.0).\n\
skip NUMBER     skip NUMBER values from DATA after each copying (default 0).\n\
silent        	don't output any of the data values.\n\
si              the input contains binary short integers.\n\
sw              reverse the bytes of the short integers on input.\n\
\n\
      Example:\n\
      vector scalar product bvec[i] = 5.0 * avec[i]:\n\
      double -scale 5.0 <avec >bvec\n\
\n\
      vector addition cvec[i] = avec[i] + bvec[i]:\n\
      double -add avec <bvec >cvec\n\
\n\
      vector multiplication cvec[i] = avec[i] * bvec[i]:\n\
      double -mult avec <bvec >cvec\n\
      \n\
      vector editing:\n\
      bvec[i] = avec[2i]\n\
      double -skip 1 <avec >bvec\n\
\n\
      bvec[i] = avec[i+5]\n\
      double -offset 5 <avec >bvec\n\
\n\
      bvec[i] = avec[2i]\n\
      bvec[i+1] = avec[2i+1]\n\
      double -copy 2 -skip 2 <avec >bvec\n\
\n\
      maximum and minimum elements of a vector:\n\
      double -max -silent <avec\n\
      double -min -silent <avec\n\
      rich@rice.edu\n\
\n\
";

int binary_output=0, float_output=0, binary_input=0;
int short_int_input=0, swap_bytes=0;
int take_derivative=0;
#define LARGE (9e30)
double last_data = LARGE;

/* readit and writeit return 1 on success and 0 on error */
int
readit (data, fp)
     double *data;
     FILE *fp;
{
  int ret;
  if (binary_input)
    {
      /* returns the number of bytes actually read or 0 for end-of-file */
      ret = read( fileno(fp), (char*)data, sizeof (double));
      return (ret > 0);
    }
  else if (short_int_input)
    {
      /* returns the number of bytes actually read or 0 for end-of-file */
      short int a;
      ret = read( fileno(fp), (char*)&a, sizeof (a));
      if (swap_bytes)
	*data = (short int)((a & 0xFF)<<8 | ((a>>8) & 0xFF));
      else
	*data = a;
      return (ret > 0);
    }
  else
    {
      /* returns EOF on error, zero if succesful */
      ret = fscanf (fp, "%lf", data);
      return (ret > 0);
    }
}

int
writeit  (data, fp)
     double *data;
     FILE *fp;
{
  int success = 0;
  if (binary_output)
    {
      /* number of bytes writen is returned.   -1 for error */
      success = (0 < write ( fileno(fp), (char*)data, sizeof (double)));
      return success;
    }
  else if (float_output)
    {
      float tmp;
      tmp = *data;
      /* number of bytes writen is returned.   -1 for error */
      success = (0 < write ( fileno(fp), (char*) &tmp, sizeof (float)));
      return success;
    }
  else
    {
#ifdef V
      /* V system: EOF is returned upon end of input, zero if succesful */
      success = (0 == fprintf (fp, "%.19g\n", *data));
#endif
#ifdef sparc
      /* sparc os4.0: fprintf returns EOF on error, else number
	 of characters written */
      int ret = fprintf (fp, "%#.19g\n", *data);
      success = (0 < ret);
#endif
#ifdef myrias
      /* myrias: fprintf returns EOF on error, else number of bytes written. */
      success = (0 < fprintf (fp, "%#.19g\n", *data));
#endif
#ifdef sun3
      /* sun3: fprintf returns EOF on error, zero if succesful */
      success = (0 <= fprintf (fp, "%#.19g\n", *data));
#endif
#ifdef sequent
      /* sequent: fprintf returns EOF on error, zero if succesful */
      success = (0 <= fprintf (fp, "%#.19g\n", *data));
#endif
      /* return zero if write failed. */
      return success;
    }
}

int
main (argc, argv)
     int argc;
     char **argv;
{
  int copy=1, offset=0, skip=0;
  char *add_file=0, *mult_file=0, *join_file=0;
  double scale=1., baseline=0.;
  FILE *add_fp = NULL;
  FILE *mult_fp = NULL;
  FILE *join_fp = NULL;
  double data=0., max = -1.0e+38, min=1.0e+38;
  int read_ok=1, write_ok=1, do_min=0, do_max=0, silent=0;

  if (argc <= 1)
    {
      printf( "%s", help_message);
      exit (0);
    }
      
  argc--;
  argv++;
  while (argc > 0)
    {
      /*
       *            OPTIONS WHICH ARE SWITCHES (NO ARGUMENT):
       */
      if (!strcmp (*argv, "-bo"))
	/*  output is binary */
	{
	  argc--; argv++;
	  binary_output = 1;
	}
      else if (!strcmp (*argv, "-fo"))
	/*  output is binary */
	{
	  argc--; argv++;
	  float_output = 1;
	}
      else if (!strcmp (*argv, "-bi"))
	/*  input is binary */
	{
	  argc--; argv++;
	  binary_input = 1;
	}
      else if (!strcmp (*argv, "-deriv"))
	/* take the derivative of the input. */
	{
	  argc--; argv++;
	  take_derivative = 1;
	}
      else if (!strcmp (*argv, "-si"))
	/*  input is binary short ints */
	{
	  argc--; argv++;
	  short_int_input = 1;
	}
      else if (!strcmp (*argv, "-sw"))
	/*  swap bytes on input */
	{
	  argc--; argv++;
	  swap_bytes = 1;
	}
      else if (!strcmp (*argv, "-h"))
	{
	  printf( "%s", help_message);
	  exit (0);
	}
      else if (!strcmp (*argv, "-max"))
	/* output maximum value at the end. */
	{
	  do_max = 1;
	  argc--; argv++;
	}
      else if (!strcmp (*argv, "-min"))
	/* output minimum value at the end. */
	{
	  do_min = 1;
	  argc--; argv++;
	}
      else if (!strcmp (*argv, "-silent"))
	/* do not output the data values. */
	{
	  silent = 1;
	  argc--; argv++;
	}
      /*
       *               OPTIONS WHICH TAKE ARGUMENTS:
       */
      else if (argc < 2)
	{
	  fprintf (stderr, "No argument following \"%s\" switch\n", *argv);
	  exit (-1);
	}
      else if (!strcmp (*argv, "-add"))
	/*  add values from FILE. */
	{
	  argc--; argv++;
	  add_file = *argv;
	  argc--; argv++;
	}
      else if (!strcmp (*argv, "-baseline"))
	/*  add BASELINE to each output value. */
	{
	  argc--; argv++;
	  baseline = atof (*argv);
	  argc--; argv++;
	}
      else if (!strcmp (*argv, "-copy"))
	/*  number of values to copy to the output each time around. */
	{
	  argc--; argv++;
	  copy = atoi (*argv);
	  argc--; argv++;
	}
      else if (!strcmp (*argv, "-join"))
	/*  merge file. */
	{
	  argc--; argv++;
	  join_file = *argv;
	  argc--; argv++;
	}
      else if (!strcmp (*argv, "-mult"))
	/*  multiply each output value by one value from FILE. */
	{
	  argc--; argv++;
	  mult_file = *argv;
	  argc--; argv++;
	}
      else if (!strcmp (*argv, "-offset"))
	/*  number of data to skip from the beginning of the file. */
	{
	  argc--; argv++;
	  offset = atoi (*argv);
	  argc--; argv++;
	}
      else if (!strcmp (*argv, "-scale"))
	/*  scale each output value by command line value. */
	{
	  argc--; argv++;
	  scale = atof (*argv);
	  argc--; argv++;
	}
      else if (!strcmp (*argv, "-skip"))
	/* number of values to skip between those we copy. */
	{
	  argc--; argv++;
	  skip = atoi (*argv);
	  argc--; argv++;
	}
      else
	{
	  fprintf (stderr, "unknown switch \"%s\"\n", *argv);
	  argc--; argv++;
	}
    }
  
  /*  open additive file. */
  if (add_file )
    {
      add_fp = fopen (add_file, "r");
      if (add_fp == NULL)
	{
	  printf ("%s: unable to open: %s\n", argv[0], add_file);
	  exit (-1);
	}
    }
  
  /*  open multiplicative scaling file. */
  if (mult_file )
    {
      mult_fp = fopen (mult_file, "r");
      if (mult_fp == NULL)
	{
	  printf ("%s: unable to open: %s\n", argv[0], mult_file);
	  exit (-1);
	}
    }
  
  
  /*  open file of data to be joined in. */
  if (join_file )
    {
      join_fp = fopen (join_file, "r");
      if (join_fp == NULL)
	{
	  printf ("%s: unable to open: %s\n", argv[0], join_file);
	  exit (-1);
	}
    }
  
  
  /*  skip data upto the offset datum. */
  while (offset-- &&  read_ok)
    {
      read_ok = readit (&data, stdin);
    }
  
  
  while (read_ok )
    {
      int count = copy;
      double mult_data, add_data;
      while (count-- &&  read_ok)
	{
	  read_ok = readit (&data, stdin);
	  if ( ! read_ok )
	    goto finish;
	  if (add_fp != NULL)
	    {
	      read_ok = readit (&add_data, add_fp);
	      if ( ! read_ok )
		goto finish;
	      data += add_data;
	    }
	  if (mult_fp != NULL)
	    {
	      read_ok = readit (&mult_data, mult_fp);
	      if ( ! read_ok )
		goto finish;
	      data *= mult_data;
	    }
	  data *= scale;
	  data += baseline;
	  if (data > max)
	    max = data;
	  if (data < min)
	    min = data;
	  
	  if (take_derivative)
	    {
	      double tmp = data;
	      if (last_data == LARGE)
		data = last_data;
	      data -= last_data;
	      last_data = tmp;
	    }
	  if (! silent)
	    {
	      write_ok = writeit (&data, stdout);
	      if ( ! write_ok)
		{
		  fprintf ( stderr, "double: unable to write to output.\n");
		  exit (-1);
		}
	      if (join_fp != NULL)
		{
		  read_ok = readit (&data, join_fp);
		  if (read_ok)
		    write_ok = writeit (&data, stdout);
		  if ( ! write_ok)
		    {
		      fprintf ( stderr, "double: unable to write to output.\n");
		      exit (-1);
		    }
		}
	    }
	}
      count = skip;
      while (count-- &&  read_ok)
	{
	  read_ok = readit (&data, stdin);
	}
    }
 finish:
  if (do_max)
    writeit (&max, stdout);
  if (do_min)
    writeit (&min, stdout);
  return 0;
}
