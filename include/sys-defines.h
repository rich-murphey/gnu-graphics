#include <stdio.h>
#include <ctype.h>
#include <math.h>

#ifdef STDC_HEADERS
#include <string.h>
#include <stdlib.h>
#else
#ifdef	USG
#include <string.h>
#else
#include <strings.h>
#endif	/* USG */

#ifdef sequent
#define NEED_DREM
#define strchr index
#define strrchr rindex
#define NO_MALLOC_H
#endif /* sequent */

#ifdef NeXT
#define NO_MALLOC_H
extern double atof(char *string);
#endif /* NeXT */

#ifdef NO_MALLOC_H
extern void *malloc();
extern void *realloc();
#else
#include <malloc.h>
#endif /* NO_MALLOC_H */


#endif	/* STDC_HEADERS */
