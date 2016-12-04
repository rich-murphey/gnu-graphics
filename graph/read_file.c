/* plot, a set of unix plot utilities.
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

/* This file is the main routine for graph. */

#include "sys-defines.h"
#include "libplot.h"
#include "extern.h"

/* read_number() reads a number form the specified stream.  The data
   spec indicates how to read the number.  read_number returns 0 upon
   end of file or error.  */

int
read_number (stream, data_spec, value)
     FILE *stream;
     data_type data_spec;
     float *value;
{
  int rc;			/* Return code		*/
  short short_val;		/* Holds short value	*/
  int int_val;			/* Holds int value	*/
  double double_val;		/* Holds double value	*/

  /*
   * Read the data according to the given data spec
   */

  switch (data_spec)
    {
    case ASCII_DATA:
      rc = fscanf (stream, "%f ", value);
      if (rc == EOF) rc = 0;
      break;
    case INT_DATA:
      rc = fread ((VOIDPTR *) & int_val, sizeof (int_val), 1, stream);
      *value = (float) int_val;
      break;
    case SHORT_INT_DATA:
      rc = fread ((VOIDPTR *) & short_val, sizeof (short_val), 1, stream);
      *value = (float) short_val;
      break;
    case FLOAT_DATA:
      rc = fread ((VOIDPTR *) value, sizeof (*value), 1, stream);
      break;
    case DOUBLE_DATA:
      rc = fread ((VOIDPTR *) & double_val, sizeof (double_val), 1, stream);
      *value = (float) double_val;
      break;
    }

  return rc;
}

/* read_file reads a file of coordinates and labels from a specified
   stream. It passes back x, y, and label arrays containing the data
   read in and the size of the arrays. It returns the number of elements
   read into the arrays. The size and number of elements read is equal
   for all three arrays. */

int
read_file (in_stream, p, length, no_of_points, auto_abscissa,
	   x_start, delta_x, data_spec, symbol_index, transpose_axes,
	   default_label, linemode, m_break_flag)
     FILE *in_stream;
     point_struct **p;
     int *length;
     int *no_of_points;
     int auto_abscissa;
     double x_start;
     double delta_x;
     data_type data_spec;
     int symbol_index;
     int transpose_axes;
     char *default_label;
     int linemode;
     int m_break_flag;		/* non-zero means break when x decreases. */
{
  char *input_string;		/* buffer to hold a label */
  int input_string_length;	/* length of input_string buffer */
  char lookahead;		/* next character to be read */
  int no_read = 1;		/* no of items last read. */
  double abscissa;		/* counter for auto_abscissa */
  int first_point;

  first_point = *no_of_points;	/* break at first point */
  abscissa = x_start;
  input_string_length = 1024;
  input_string = (char *) do_malloc (input_string_length);

  while (!feof (in_stream) && (no_read > 0))
    {
      /*
       * Grow the buffer if needed
       */
      if (*no_of_points >= *length)
	{
	  *length *= 2;
	  (*p) = (point_struct *) do_realloc ((*p), *length * sizeof (point_struct));
	}
      /*
       * Read in the data
       */
      if (auto_abscissa)
	{
	  if (transpose_axes)
	    (*p)[*no_of_points].y = abscissa;
	  else
	    (*p)[*no_of_points].x = abscissa;
	  abscissa += delta_x;
	}
      else
	{
	  if (transpose_axes)
	    no_read = read_number (in_stream, data_spec, &(*p)[*no_of_points].y);
	  else
	    no_read = read_number (in_stream, data_spec, &(*p)[*no_of_points].x);
	  if (no_read == 0 && data_spec == ASCII_DATA)
	    {
	      char *newline;

	      fgets (input_string, input_string_length, in_stream);
	      newline = strchr(input_string, '\n');
	      if (newline != NULL)
		*newline = '\0';
	      (void) fprintf (stderr,
			      "%s:  error reading x coordinate at `%s'.\n",
			      progname, input_string);
	      exit (-1);
	    }
	}
      if (transpose_axes)
	no_read = read_number (in_stream, data_spec, &(*p)[*no_of_points].x);
      else
	no_read = read_number (in_stream, data_spec, &(*p)[*no_of_points].y);
      if (no_read == 0 && data_spec == ASCII_DATA)
	{
	  char *s;
	  fgets (input_string, input_string_length, in_stream);
	  s = strchr (input_string, '\n');
	  if (s) *s = '\0';
	  fprintf (stderr, "%s:  error reading y coordinate at `%s'.\n",
		   progname, input_string);
	  exit (-1);
	}

      /* If we are reading ascii data, look for a label as well. */
      if (data_spec != ASCII_DATA)
	(*p)[*no_of_points].string = default_label;
      else
	{
	  lookahead = getc (in_stream);
	  ungetc (lookahead, in_stream);
	  if (isdigit (lookahead)
	      || (lookahead == EOF)
	      || (lookahead == '.')
	      || (lookahead == '+')
	      || (lookahead == '-'))
	    {
	      (*p)[*no_of_points].string = default_label;
	    }
	  else
	    {
	      int i = 0, again = 0, quoted = 0;
	      char last_char;

	      if (lookahead == '"')
		{
		  quoted = 1;
		  getc (in_stream);	/* skip double quote */
		}
	      else
		quoted = 0;

	      do
		{
		  last_char = getc (in_stream);
		  input_string[i] = last_char;
		  if (i >= (input_string_length - 1))
		    {
		      input_string_length *= 2;
		      input_string = (char *) do_realloc
			(input_string, input_string_length * sizeof (char *));
		    }
		  if (last_char == '\\') /* backquote is the escape char */
		    {
		      last_char = getc (in_stream);
		      input_string[i] = last_char;
		      last_char = 0;	/* ignore the last char
					   since it was escaped */
		    }
		  i++;
		  if (quoted)
		    {
		      if (last_char != '"')
			again = 1;
		      else
			again = 0;
		    }
		  else
		    {
		      switch (last_char)
			{
			case ' ':
			case '\t':
			case '\n':
			case '\v':
			case '\f':
			  again = 0;
			  break;
			default:
			  again = 1;
			}
		    }
	      } while (again && (last_char != EOF));

	      input_string[i - 1] = '\0'; /* replace termination with null */
	      (*p)[*no_of_points].string
		= (char *) do_malloc (strlen (input_string));
	      strcpy ((*p)[*no_of_points].string, input_string);
	    }
	}
      (*p)[*no_of_points].symbol = symbol_index;
      if (no_of_points)
	{
	  if (   m_break_flag
	      && ((*p)[*no_of_points-1].x > (*p)[*no_of_points].x))
	    {
	      (*p)[*no_of_points].linemode = -1;	    
	      if (m_break_flag == 2)
		linemode++;
	    }
	  else
	    (*p)[*no_of_points].linemode = linemode;
	}
      if (no_read > 0)
	(*no_of_points)++;
    }

  if (*no_of_points > first_point) /* break the line at the first point */
    (*p)[first_point].linemode = -1;
  free (input_string);
  return *no_of_points;
}
