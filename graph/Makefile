# This is the Makefile for graph, a program which generates plot files.
defines = # -DSTDC_HEADERS -DHAVE_VOIDPTR -ansi
#CC = gcc
#COPTS = -g -Wall
CC = cc
COPTS = -O
CFLAGS = $(COPTS) $(defines)
LDFLAGS = -g
LDLIBS=-lm
GETOPT=../getopt
#----------------------------------------------------------------
# The toplevel make file uses these.
.PHONY: all install clean realclean

all : graph

VERS=0.15
.c.o:
	$(CC) $(CFLAGS) -I$(GETOPT) -I../include -DVERS=\"$(VERS)\" -c $<

graph : graph.o read_file.o max.o write_plot.o $(GETOPT)/getopt.o $(GETOPT)/getopt1.o ../tek2plot/libplot.a
	$(CC) $(CFLAGS) graph.o read_file.o max.o write_plot.o $(GETOPT)/getopt.o $(GETOPT)/getopt1.o ../tek2plot/libplot.a $(LDLIBS) -o graph

../tek2plot/libplot.a :
	cd ../tek2plot; make libplot.a CC="$(CC)" CFLAGS="$(CFLAGS)"

$(GETOPT)/getopt.o $(GETOPT)/getopt1.o : $(GETOPT)/getopt.c $(GETOPT)/getopt1.c
	cd $(GETOPT); make all CC="$(CC)" CFLAGS="$(CFLAGS)"

graph.o : graph.c extern.h ../include/libplot.h 
max.o : max.c extern.h 
read_file.o : read_file.c extern.h 
write_plot.o : write_plot.c ../include/libplot.h extern.h 
$(GETOPT)/getopt.o : $(GETOPT)/getopt.h $(GETOPT)/getopt.h
$(GETOPT)/getopt1.o : $(GETOPT)/getopt.h $(GETOPT)/getopt.h
#----------------------------------------------------------------
TAGS : graph.c read_file.c max.c write_plot.c extern.h ../include/libplot.h
	etags graph.c read_file.c max.c write_plot.c extern.h ../include/libplot.h
install :
	install -c graph $(BINDIR)

clean :
	-rm -f *.o *~ \#* core test* data* TAGS
realclean : clean
	-rm -f graph *.a
