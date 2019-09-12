#ifndef _SYS_DEFINES_H_
#define _SYS_DEFINES_H_ 1
#include "config.h"

#ifdef HAVE_SYS_TYPES_H
#  include <sys/types.h>
#endif

#ifdef STDC_HEADERS
#  include <stdlib.h>
#  include <string.h>
#else
   extern char *getenv (const char *name);
   extern int atoi (const char *nptr);
   extern double atof (const char *nptr);
#  define	EXIT_FAILURE	1
#  define	EXIT_SUCCESS	0
#  ifdef HAVE_STRCHR
#    ifdef HAVE_STRING_H
#      include <string.h>
#    else
#      ifdef HAVE_STRINGS_H
#        include <strings.h>
#      endif
#    endif
#  else
#    ifdef HAVE_STRINGS_H
#      include <strings.h>
#    else
#      ifdef HAVE_STRING_H
#        include <string.h>
#      endif
#    endif
#    define strchr index
#    define strrchr rindex
#  endif
#  ifndef HAVE_MEMCPY
#    define memcpy(d, s, n) bcopy ((s), (d), (n))
#  endif
#  ifndef HAVE_MEMMOVE
#    define memmove(d, s, n) bcopy ((s), (d), (n))
#  endif

#  ifdef HAVE_MALLOC_H
#    include <malloc.h>
#  else
     extern void * malloc (size_t size);
     extern void * realloc (void * ptr, size_t size);
     extern void * calloc (size_t nmemb, size_t size);
     extern void free (void * ptr);
#  endif
#endif

#ifdef STDC_HEADERS
# include <stdlib.h>
# include <stddef.h>
#else
# ifdef HAVE_STDLIB_H
#  include <stdlib.h>
# endif
#endif

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#include <stdio.h>
#include <ctype.h>
#include <errno.h>

#ifdef __DJGPP__
#  define _USE_LIBM_MATH_H
#endif

#include <math.h> 
#ifdef HAVE_FLOAT_H
#  include <float.h>
#endif
#ifdef HAVE_LIMITS_H
#  include <limits.h>
#endif
#ifdef HAVE_VALUES_H
#  include <values.h>
#endif

#ifndef NULL
#  define NULL 0
#endif

#ifdef __STDC__
   typedef enum { false = 0, true = 1 } bool;
#else
   typedef int bool;
#  define false 0
#  define true 1
#endif

#endif
