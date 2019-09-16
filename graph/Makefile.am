bin_PROGRAMS = graph
graph_SOURCES = graph.c read_file.c max.c write_plot.c
graph_LDADD =  ../libplot/libplot.la -lm
if BUILD_GETOPT
  graph_LDADD += ../libgetopt/libgetopt.la
else
  graph_LDADD +=
endif
AM_CPPFLAGS = -I$(srcdir)/../include
