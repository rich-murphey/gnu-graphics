bin_PROGRAMS = plot2fig
plot2fig_SOURCES = plot2fig.c alabel.c arc.c circle.c closepl.c color.c cont.c dot.c draw_line.c erase.c fill.c fontname.c fontsize.c label.c line.c linemod.c move.c openpl.c point.c rotate.c space.c 
AM_CPPFLAGS = -I$(srcdir)/../include
if BUILD_GETOPT
  plot2fig_LDADD =  ../libgetopt/libgetopt.la
else
  plot2fig_LDADD =
endif
