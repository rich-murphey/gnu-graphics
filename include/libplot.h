/* plot, a library of functions for tektronics 4010 compatible devices.
   Copyright (C) 1989 Free Software Foundation, Inc.

libtek is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY.  No author or distributor accepts responsibility to anyone for the
consequences of using it or for whether it serves any particular purpose or
works at all, unless he says so in writing.  Refer to the GNU General Public
License for full details.

Everyone is granted permission to copy, modify and redistribute libtek, but
only under the conditions described in the GNU General Public License.  A copy
of this license is supposed to have been given to you along with libtek so
you can know your rights and responsibilities.  It should be in a file named
COPYING.  Among other things, the copyright notice and this notice must be
preserved on all copies.  */

#ifndef _libplot_defines_
#define _libplot_defines_ 1
#include <stdio.h>

/* This elides the argument prototypes if the compiler does not
   supprort them. The name protarg is chosen in hopes that it will not
   collide with any others. */
#ifdef __STDC__
#define P__(a)	a
#else
#define P__(a)	()
#endif

/* the standard library of plot functions */
extern int arc P__ ((int xc, int yc, int x0, int y0, int x1, int y1));
extern int box P__ ((int x0, int y0, int x1, int y1));
extern int circle P__ ((int x, int y, int r));
extern int closepl P__ ((void));
extern int cont P__ ((int x, int y));
extern int erase P__ ((void));
extern int label P__ ((char *s));
extern int line P__ ((int x0, int y0, int x1, int y1));
extern int linemod P__ ((char *s));
extern int move P__ ((int x, int y));
extern int openpl P__ ((void));
extern int point P__ ((int x, int y));
extern int space P__ ((int x0, int y0, int x1, int y1));

/* Extensions to the library for postscript */
extern int alabel P__ ((int x_justify, int y_justify, char *s));
extern int color P__ ((int red, int green, int blue));
extern int dot P__ ((int x, int y, int dx, int n, char *pattern));
extern int fill P__ ((int level));
extern int fontname P__ ((char *s));
extern int fontsize P__ ((int size));
extern void putshort P__ ((int x));
extern int rotate P__ ((int x, int y, int angle));

/* Internal routines: */
extern int find_font P__ ((char *s));
extern int string_width P__ ((char *s));
extern int draw_line P__ ((void));
extern void find_byte_order P__ ((int *x0, int *y0, int *x1, int *y1));
extern int coord P__ ((FILE *input));
extern void read_string P__ ((FILE *input, char *buffer, int buffer_length));
extern void read_plot P__ ((FILE *in_stream, char *buffer, int buffer_length));
extern int swap_bytes P__ ((int x));
extern void set_range P__ ((double x, double y));
extern void get_range P__ ((double *xmin, double *xmax, double *ymin, double *ymax));
#undef P__

/* Global variables which affect the output: */
extern double x_input_min; /* minimum input x coordinate */
extern double y_input_min; /* minimum input y coordinate */
extern double x_output_min; /* minimum output x coordinate */
extern double y_output_min; /* minimum output y coordinate */
extern double x_output_max; /* maximum output x coordinate */
extern double y_output_max; /* maximum output y coordinate */
extern double scaleup; /* input to output scaleing for both x and y */

extern int last_x, last_y; /* location of the last coordinates used */

/* This is the default scaling in the postscript current transformation
   matrix (CTM) for the entire plot. This value is arbitrarily chosen
   for compatibility with idraw. See open.c. */
#define SCALING 0.8

/* The scaling and rotation part of a postscript transformation matrix */
extern double text_transformation_matrix[4];

/* Note: define MAX_NO_OF_POINTS according to the number of
   points the postscript device can handle.  Experimentation has
   shown 150 to be reasonable. */

#define MAX_NO_OF_POINTS 150
extern int line_x[MAX_NO_OF_POINTS];
extern int line_y[MAX_NO_OF_POINTS];

/* We accumulate coordinates untill we either have enought points
   or the line is broken.  This counter tells us whether we have
   accumulated points and how many there are. */

extern int PointsInLine;

/* This bit vector represents the line style (eg. dashed) for
   idraw. We intitialize it to all ones which represents a solid
   line. */

extern long line_type_bit_vector;

/* This is a string that should conatain a postscript vector
   argument for the postscript setdash command.  This is allocted
   in the OPENPL function. */

extern char *line_type_setdash;

/* The current length of the above buffer */

extern int line_type_setdash_length;

/* LINE_TYPE_SETDASH_BITS is one greater than the length in number of bits in
   the dash pattern.  It can be computed as the sum of the integers contained
   in LINE_TYPE_SETDASH plus one.  If the string is empty, then the value should
   be zero. */

extern int line_type_setdash_bits;

/* Idraw puts the upper left corner of the tex at the current */

/* TEXT_ROTATION is the angle in degress counterclockwise from the
   horizontal for rotation of labels. */

extern int text_rotation;

/* FONT_SIZE is the font size in printer's points (-f option). */

extern double font_size;

/* FONT_SCALING takes into account the font scaling by idraw.  Version 2.4
   of idraw scales the whole drawing down by a factor of SCALING, we must
   scale the fonts back up by 1.25 so that a 14 point fonts appears as
   a 14 point font on the hardcopy. Version 2.5 of idraw does not seem
   to need this adjustment, but it is left here for compatibility. */

extern double font_scaling;

/* USER_HAS_PROLOGUE is a flag. If it is non-zero then the OPENPL routine
   should output the user specified prologue contained in the file specified
   in the string USERS_PROLOGUE */

extern int user_has_prologue;

/* USERS_PROLOGUE is a string containing the file name for any user specified
   postscript prologue.  This file is a substitute for the prologue contained
   in the file header.c. */

char *users_prologue;


struct font_info_struct {
  char *name;			/* the name the user specifies the font by */
  char *X_name;			/* the X windows font name */
  char *ps_name;		/* the postscript font name */
  int width[256];		/* the font width information */
};

#define NO_OF_FONTS 13

extern struct font_info_struct font_info[NO_OF_FONTS];


/* CURRENT_FONT is a string containing the name of the current font. */

extern char* current_font;

/* DEFAULT_FONT is a string containing the name of the default font.
   This string should be a valid match for one of the font names. */

extern char* default_font;

/* FIND_FONT takes a string argument S containing a font name and returns the
   index of the font structure FONT_INFO corresponding to that name. If no match
   is found for the font name, an error message is printed and the index of the
   default font is returned. */

extern int find_font();


/* FGCOLOR_RED, FGCOLOR_GREEN and FGCOLOR_BLUE indicate the foreground
   color of all plot. For each the range of intensity is from 0 to 1 so
   that a value of (0,0,0) represents black and a value of (1,1,1)
   indicates white. */

extern double fgcolor_red, fgcolor_green, fgcolor_blue;

/* FILL_LEVEL is the intensity of the filler for closed paths.  Intensity
   ranges from 0 to 1. A value of 0 represents black and a value of 1 indicates
   white. */

extern double fill_level;

/* OUTPUT_HIGH_BYTE_FIRST specifies the byte order for two byte integers
   written to the output.

   A value of 1 specifies that they will be written high byte first.
   A value of -1 specifies that they will be written low byte first.
   A zero value specifies that the host (default) byte order is used. */

extern int output_high_byte_first;

/* LINE_WIDTH is the interger value of width of lines drawn. */

extern int line_width;

/* some machines do not define M_PI. */
#ifndef M_PI
#define M_PI 3.14159265358979323846264
#endif

#endif /* _libplot_defines_ */
