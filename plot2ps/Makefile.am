bin_PROGRAMS = plot2ps
plot2ps_SOURCES = plot2ps.c alabel.c arc.c box.c circle.c close.c color.c cont.c dot.c drawline.c erase.c fill.c fontsize.c header.c label.c line.c linemod.c move.c open.c point.c range.c rotate.c setfont.c space.c
plot2ps_LDADD = ../libplot/libplot.la -lm
AM_CPPFLAGS = -I$(srcdir)/../include
if BUILD_GETOPT
  plot2ps_LDADD += ../libgetopt/libgetopt.la
else
  plot2ps_LDADD +=
endif
