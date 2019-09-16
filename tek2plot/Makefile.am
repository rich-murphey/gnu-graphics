bin_PROGRAMS = tek2plot
tek2plot_SOURCES = tek2plot.c
tek2plot_LDADD = ../libplot/libplot.la -lm
AM_CPPFLAGS = -I$(srcdir)/../include
if BUILD_GETOPT
  tek2plot_LDADD += ../libgetopt/libgetopt.la
else
  tek2plot_LDADD +=
endif
