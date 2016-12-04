/*
 * $Header: /files1/home/toy/src/stat/RCS/gps.h,v 1.6 90/11/04 18:17:33 toy Exp $
 *
 * $Log:	gps.h,v $
 * Revision 1.6  90/11/04  18:17:33  toy
 * Added prototype for next_column.
 *
 * Revision 1.5  90/09/09  14:00:27  toy
 * Added declarations/prototypes for print_help_strings, set_def_prec,
 * set_f_format, and set_g_format.
 *
 * Revision 1.4  90/09/02  17:16:33  toy
 * Added prototype for read_vector.
 *
 * Revision 1.3  90/09/02  11:16:53  toy
 * New definition of get_my_name.
 *
 * Revision 1.2  90/09/01  18:11:10  toy
 * Added declarations for the new library routines for reading
 * and writing numbers and for printing messages.
 *
 * Revision 1.1  90/08/31  14:50:59  toy
 * Initial revision
 *
 */

#ifndef	_GPS
#define	_GPS

#if	defined(__STDC__) || defined(__GNUC__)
#undef	PARMS
#define	PARMS(x)	x
#else
#define	PARMS(x)	()
#define	const
#endif

#define	TRUE	1
#define	FALSE	0

extern const char *progname;

extern char *get_my_name PARMS ((char *argv, const char *def_name));
extern int get_number PARMS ((FILE * fp, double *value));
extern void print_number PARMS ((FILE * fp, double value));
extern void print_help_strings PARMS ((const char *str_array[]));
extern int set_max_columns PARMS ((int max));
extern int set_precision PARMS ((int prec));
extern void set_def_prec PARMS ((void));
extern void end_column PARMS ((FILE * fp));
extern void message PARMS ((const char *format,...));
extern double *read_vector PARMS ((FILE * fp, int *rank));
extern void set_f_format PARMS ((void));
extern void set_g_format PARMS ((void));
extern void next_column PARMS ((FILE * fp));

#endif
