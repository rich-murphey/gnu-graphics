## Makefile.am for plotutils.
## Process this file with automake to produce Makefile.in in all directories.

# plot, tek2plot, plotfont, graph, and hersheydemo are linked with libplot.
# spline, double, and ode are not, however.  All use the routines in lib.
#
# pic2plot is written in C++ but is currently linked with libplot, not
# libplotter.  (It could easily be modified to use libplotter rather than
# libplot.)  We build it only if --enable-libplotter is specified, though;
# which is a bit contradictory.

# Since the libxmi 2D rasterization library is distributed as a separate
# package, by default we don't build and install it as a standalone
# library.  (It's always compiled into libplot and libplotter, though.)

SUBDIRS = getopt libplot xplot
## lib spline double ode ode-examples libplot $(ADD_LIBXMI) plot tek2plot plotfont graph hersheydemo $(ADD_LIBPLOTTER) include info doc fonts test

# files with nonstandard names in this directory
# EXTRA_DIST = Changelog

# remove files that are symlinks, and shouldn't be part of the 
# distribution
# dist-hook:
# 	rm -f $(distdir)/libplotter/*.cc
# 	rm -f $(distdir)/libplotter/*.h
# 	for i in graph plot tek2plot plotfont pic2plot; do rm -f $(distdir)/$$i/fontlist.c; done
