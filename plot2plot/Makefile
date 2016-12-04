# This is the Makefile for plot2plot, a plot file to plot file converter.
#----------------------------------------------------------------
CC = gcc
CFLAGS = -g -Wall # -ansi -DSTDC_HEADERS -DHAVE_VOIDPTR
LDFLAGS = -g
GETOPT = ../getopt
# The toplevel make file uses these.
.PHONY: all install clean realclean

all : plot2plot

VERS=0.1
.c.o:
	$(CC) $(CFLAGS) $(DEF) -I../getopt -I../include -DVERS=\"$(VERS)\" -c $<

install : plot2plot
	install -c plot2plot $(BINDIR)
#
# Type `make clean' to clean up after compilation.
#
clean:
	-rm -f *.o *~ \#* core test* diff TAGS CHECKSUMS test.output
realclean : clean
	-rm -f plot2plot *.a libplot.h
#----------------------------------------------------------------
#
# Type `make plot2plot' to compile plot2plot.
#
plot2plot : plot2.o $(GETOPT)/getopt.o $(GETOPT)/getopt1.o ../tek2plot/libplot.a
	$(CC) $(CFLAGS) -o plot2plot plot2.o $(GETOPT)/getopt.o $(GETOPT)/getopt1.o ../tek2plot/libplot.a -lm

../tek2plot/libplot.a :
	cd ../tek2plot; make libplot.a CC=$(CC) CFLAGS=$(CFLAGS)

$(GETOPT)/getopt.o $(GETOPT)/getopt1.o : $(GETOPT)/getopt.c $(GETOPT)/getopt1.c
	cd $(GETOPT); make all CC="$(CC)" CFLAGS="$(CFLAGS)"

plot2.o : plot2.c ../include/libplot.h
