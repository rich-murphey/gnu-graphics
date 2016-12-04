# This is the top level make file for GNU graphics.

# How to install graphics:

# 1) Check the CC and CFLAGS variables and type `make'.

# 2) To build the X window plot previewer, You may have to specify the
# location of the X include files and libraries in the file
# xplot/Makefile.

# 3) Edit BINDIR, MANDIR, MANEXT and INFODIR and type `make install'
# to install the executables.

# 4) To clean up or run the tests again:

#    `make test' will run regression tests on the executables.

#    `make clean' will clean up intermediate files but leave the executables.

#    `make realclean' will remove executables TAGS files etc. as well

#----------------------------------------------------------------
#    Specify CC and CFLAGS according to your machine architecture.
#----------------------------------------------------------------
# sun3, sun4, sequent, mips, next, and hp9000:
CC=cc
CFLAGS=-O

# ultrix:
#CC=gcc
#CFLAGS=-O

# for debugging:
#CC=gcc
#CFLAGS="-g -DDEBUG"

#----------------------------------------------------------------
#    Specify the directories where binaries and documentation are installed:
#----------------------------------------------------------------

#    Where do we put the executables?
BINDIR=/omicron2/rich/bin.sun4/gnu

#    Where do we put the manual pages?
MANDIR=/omicron2/rich/man/man1

#    What is the filename extension for the manual pages?
MANEXT=1

#    Where do we put the online info-ized texinfo documentation?
INFODIR=/kappa/ftp/pub/gnuemacs/info

#----------------------------------------------------------------
VERS=0.17
MFLGS=CC=$(CC) CFLAGS="$(CFLAGS)" BINDIR=$(BINDIR) \
  MANDIR=$(MANDIR) MANEXT=$(MANEXT) INFODIR=$(INFODIR)
files=README Makefile COPYING
dirs=getopt graph plot2tek tek2plot plot2ps plot2fig plot2plot xplot

default : all test

.PHONY: all test install clean realclean

install :: graphics.info
	-mkdir $(BINDIR) $(MANDIR) $(INFODIR)
	-cp graphics.info* $(INFODIR)

graphics.info : graphics.texi
	makeinfo graphics.texi

all install clean realclean ::
	for dir in $(dirs);				\
	do						\
	  echo Making $@ in $${dir} ;			\
	  (cd $${dir}; make $(MFLGS) $@);		\
	done

dist : realclean
	cd ..; tar -czf graphics-$(VERS).tar.Z graphics-$(VERS)
	mv ../graphics-$(VERS).tar.Z /qed/ftp/pub
	cd /qed/ftp/pub; rm graphics.tar.Z; ln -s graphics-$(VERS).tar.Z graphics.tar.Z

realclean clean ::
	-rm -f *~ *.info TAGS
	cd test; make clean

test : all
	cd test; make test
xplot :
	cd $@; make $(MFLGS)
