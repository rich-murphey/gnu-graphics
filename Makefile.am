ACLOCAL_AMFLAGS = -I m4
SUBDIRS = libplot
if BUILD_GETOPT
  SUBDIRS += 
else
  SUBDIRS += libgetopt
endif
SUBDIRS += dist-stat libplot graph xplot tek2plot double plot2fig plot2plot plot2ps plot2tek test

README: README.md

test:
	(cd test; make test)
