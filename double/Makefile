defines =
CC = gcc
COPTS = -g -Wall # -ansi
CFLAGS = $(COPTS) $(defines)
LDFLAGS = -g
LDLIBS=-lm
GETOPT=../getopt
#----------------------------------------------------------------
# The toplevel make file uses these.
.PHONY: all install clean realclean

PROGRAM = double
SRC = double.c
OBJS = double.o

all : $(PROGRAM)

VERS=0.13
.c.o:
	$(CC) $(CFLAGS) -I$(GETOPT) -I../include -DVERS=\"$(VERS)\" -c $<

clean:
	$(RM) \#* *~ *.o

$(PROGRAM) : $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(PROGRAM)

install : $(PROGRAM) $(PROGRAM).man
	install -s $(PROGRAM) $(BINDIR)
	install $(PROGRAM).man $(MANDDIR)/$(PROGRAM).$(MANEXT)
	
