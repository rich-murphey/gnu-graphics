bin_PROGRAMS = plot2plot
plot2plot_SOURCES = plot2plot.c
plot2plot_LDADD = ../libplot/libplot.la
AM_CPPFLAGS = -I$(srcdir)/../include
if BUILD_GETOPT
  plot2plot_LDADD += ../libgetopt/libgetopt.la
else
  plot2plot_LDADD +=
endif
