/* Copyright (C) 1989 Free Software Foundation, Inc.

   plot is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY.  No author or distributor accepts responsibility to
   anyone for the consequences of using it or for whether it serves any
   particular purpose or works at all, unless he says so in writing.
   Refer to the GNU General Public License for full details.

   Everyone is granted permission to copy, modify and redistribute
   plot, but only under the conditions described in the GNU General
   Public License.  A copy of this license is supposed to have been
   given to you along with plot so you can know your rights and
   responsibilities.  It should be in a file named COPYING.  Among
   other things, the copyright notice and this notice must be preserved
   on all copies.  */

#include "sys-defines.h"

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Label.h>

#include "libplot.h"

extern Display *dpy;
extern Pixmap pixmap;
extern GC gc;

#ifdef DEBUG
extern int debug_flag;		/* nonzero means output debugging info. */
#endif

extern int xmin, xmax, ymin, ymax;
extern double xfactor, yfactor;

#define MAPX(x) ((int) (((x) - xmin) * xfactor))
#define MAPY(y) ((int) ((ymax - (y)) * yfactor))

double font_size = 14.;
char *font_name = NULL;
int font_name_length = 0;
char *user_font_name = NULL;	/* user specified font name */
int user_font_name_length = 0;
char *x_name = NULL;		/* x font name */
int x_name_length = 0;


struct name_info_struct
{
  char *plot;
  char *x;
};

struct name_info_struct name_info[] = {
  {
    "courier-bold",
    "courier-bold-r"},
  {
    "courier-boldoblique",
    "courier-bold-o"},
  {
    "courier-oblique",
    "courier-medium-o"},
  {
    "courier",
    "courier-medium-r"},
  {
    "helvetica-bold",
    "helvetica-bold-r"},
  {
    "helvetica-boldoblique",
    "helvetica-bold-o"},
  {
    "helvetica-oblique",
    "helvetica-medium-o"},
  {
    "helvetica",
    "helvetica-medium-r"},
  {
    "symbol",
    "symbol-medium-r"},
  {
    "times-bold",
    "times-bold-r"},
  {
    "times-bolditalic",
    "times-bold-i"},
  {
    "times-italic",
    "times-medium-i"},
  {
    "times-roman",
    "times-medium-r"},
  {
    NULL,
    NULL}
};

/* font structures necessary for finding the bounding boxes */
XFontStruct *font_struct = NULL;
int direction_return;
int font_ascent_return, font_descent_return;
XCharStruct overall_return;

int
alabel (x_justify, y_justify, s)
     char x_justify, y_justify;
     char *s;
{
  int alabel_width, alabel_height, alabel_decent;
  double x_char_offset, y_char_offset;

  switch (x_justify)		/* placement of label with respect
				   to x coordinate */
    {
    case 'l':			/* left justified */
      x_char_offset = 0.0;
      break;

    case 'c':			/* centered */
      x_char_offset = -0.5;
      break;

    case 'r':			/* right justified */
      x_char_offset = -1.0;
      break;
    }

  switch (y_justify)		/* placement of label with respect
				   to y coordinate */
    {
    case 'b':			/* bottom */
      y_char_offset = 0.0;
      break;

    case 'c':			/* centered */
      y_char_offset = 0.5;
      break;

    case 't':			/* top */
      y_char_offset = 1.0;
      break;
    }

  if (font_struct == NULL)
    {
#ifdef DEBUG
      if (debug_flag)
	fprintf(stderr, "warning: null font_struct\n");
#endif
      alabel_width = alabel_height = alabel_decent = 0;
    }
  else
    {
      alabel_width = XTextWidth (font_struct, s, strlen (s));
      XTextExtents (font_struct, s, strlen (s), &direction_return,
		    &font_ascent_return, &font_descent_return, &overall_return);
      alabel_height = font_ascent_return + font_descent_return;
      alabel_decent = font_descent_return;
    }

  /* `+ .5' encourages round off rather than truncate by (int) cast. */
  {
    int x, y;
    x = (int) (MAPX (last_x) + x_char_offset * alabel_width + .5);
    y = (int) (MAPY (last_y) + y_char_offset * alabel_height - alabel_decent + .5);
#ifdef DEBUG
    if (debug_flag)
      fprintf(stderr, "drawstring %d %d %d %s\n", gc, x, y, s);
#endif
    XDrawString (dpy, pixmap, gc, x, y, s, strlen (s));
  }
  return 0;
}

int set_font_and_size (name, size)
     char *name;
     int size;
{
  int name_size;
  int i;

#ifdef DEBUG
  if (debug_flag)
    fprintf(stderr, "fontname `%s' size %d\n", name ? name : "", size);
#endif

  /* If the name is null or zero length, use the default: */

  if ((name == NULL)
      || (strlen(name) == 0))
    return set_font_and_size (XGetDefault (dpy, "xplot", "font"), 0);

  if (size == 0)
    {
      if (font_struct)
	XFreeFont (dpy, font_struct);
      font_struct = XLoadQueryFont (dpy, name);
      if (font_struct)		/* if the font does exist */
	{
	  XSetFont (dpy, gc, font_struct->fid);
#ifdef DEBUG
	  if (debug_flag)
	    fprintf(stderr, "Font `%s' %d\n", name, font_struct);
#endif
	  return 1;		/* font found */
	}
      return 0;			/* font not found */
    }

  /* Try buildin an x font name from the name and size: */

  /* save the user specified name for later use */
  if (user_font_name_length < strlen (name) + 256)
    {
      user_font_name_length = strlen (name) + 256;
      if (user_font_name)
	user_font_name = (char *) realloc (user_font_name,
					   user_font_name_length);
      else
	user_font_name = (char *) malloc (user_font_name_length);
    }
  strcpy (user_font_name, name);


  /* search for the X font name correxponding to the requested name */
  i = -1;
  while (name_info[++i].plot)
    {
      if (strcmp (name_info[i].plot, name) == 0)
	break;
    }

  /* if the X font name is found, use it.  Otherwise, assume the supplied
     name is an X font name itself. */
  if (name_info[i].plot)
    {
      if (!x_name)
	x_name = malloc (256);
      sprintf (x_name, "*-%s-*-%d-*", name_info[i].x, (int) font_size);
      if (set_font_and_size (x_name, 0))
	return 1;
    }
  else
    {
      /* Try using the name as an X font name: */
      if (set_font_and_size (name, 0))
	return 1;
    }

  set_font_and_size (0, 0);	/* last resort: punt and use the default. */
  return 0;
}

int
fontname (name)
     char *name;
{
  if (!set_font_and_size (name, (int) font_size))
    fprintf(stderr, "warning: font `%s' not found.\n", user_font_name);
  return 0;
}

int
fontsize (points)
     int points;
{
  font_size = points;
  if (!set_font_and_size (user_font_name, (int) font_size))
    fprintf(stderr , "warning: font `%s' at size %d not found.\n",
	    user_font_name, points);
  return 0;
}

/* We cannot implement fill untill we implement saving at least
   the last contiguous line drawn.  If we save the whole plot file
   in a buffer, we could find the path by looking back after each 
   move command.  Then we could implement fill by using XFillPolygon
   on the last contiguous set of continue commands. */

int
fill (pattern)
     int pattern;
{
  return 0;
}

/* Untill we have fill, color wont make much difference. */

int
color (red, green, blue)
     int red, green, blue;
{
  return 0;
}

/* I don't know how we could rotate text yet.  Using InterViews is probably
   an easier way of getting this implemented in a reasonable ammount of time. */

int
rotate (angle, width, height)
     int angle, width, height;
{
  return 0;
}
