bin_PROGRAMS = plot2tek
plot2tek_SOURCES = plot2tek.c alabel.c arc.c circle.c closepl.c color.c cont.c dot.c erase.c fill.c fontname.c fontsize.c label.c line.c linemod.c move.c openpl.c point.c rotate.c space.c
plot2tek_LDADD = ../libplot/libplot.la -lm
AM_CPPFLAGS = -I$(srcdir)/../include
if BUILD_GETOPT
  plot2tek_LDADD += ../libgetopt/libgetopt.la
else
  plot2tek_LDADD +=
endif
