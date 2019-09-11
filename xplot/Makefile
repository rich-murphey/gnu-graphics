  # This is the Makefile for xplot, an X11 plot file previewer.
# You may have to edit LDLIBS or CFLAGS to indicate the paths to
# your X11 libs and includes if your compiler does not find them
# by default.
#
LDLIBS = -lXaw -lXmu -lXt -lXext -lX11 -lm
#
# If they aren't in /usr/lib or /usr/local/lib, you might have
# to supply full path names something like:
#
# LDLIBS = /usr/foo/lib/libXaw.a /usr/foo/lib/libXmu.a ...
#
#----------------------------------------------------------------
GETOPT=../getopt
#CC=gcc
#CFLAGS=-g -DDEBUG -DNOSTDHDRS -fstrength-reduce -fpcc-struct-return -fwritable-strings
CC=cc
CFLAGS=-O -DSTDC_HEADERS
#CFLAGS=-O -DDEBUG
#----------------------------------------------------------------
# The toplevel make file uses these.
.PHONY: all install clean realclean
VERS=0.4

all : xplot

install :
	install -c xplot $(BINDIR)
	install -c xplot.man $(MANDIR)/xplot.$(MANEXT)

clean :
	-rm -f \#* *.o *~
realclean : clean
	-rm -f xplot *.a
#----------------------------------------------------------------
.c.o:
	$(CC) $(CFLAGS) -I$(GETOPT) -I../include -DVERS=\"$(VERS)\" -c $<

xplot : xplot.o alabel.o $(GETOPT)/getopt.o $(GETOPT)/getopt1.o
	$(CC) $(CFLAGS) xplot.o alabel.o  $(GETOPT)/getopt.o $(GETOPT)/getopt1.o $(LDLIBS) -o xplot

$(GETOPT)/getopt.o $(GETOPT)/getopt1.o : $(GETOPT)/getopt.c $(GETOPT)/getopt1.c
	cd $(GETOPT); make all CC="$(CC)" CFLAGS="$(CFLAGS)"
$(GETOPT)/getopt.o : $(GETOPT)/getopt.h $(GETOPT)/getopt.h
$(GETOPT)/getopt1.o : $(GETOPT)/getopt.h $(GETOPT)/getopt.h

alabel.o : alabel.c 
xplot.o : xplot.c 
