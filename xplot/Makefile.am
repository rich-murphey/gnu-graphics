bin_PROGRAMS = xplot
xplot_SOURCES = xplot.c alabel.c
xplot_LDADD = -lX11 -lXt -lXaw -lm
AM_CPPFLAGS = -I$(srcdir)/../include
if BUILD_GETOPT
  xplot_LDADD += ../libgetopt/libgetopt.la
else
  xplot_LDADD +=
endif
