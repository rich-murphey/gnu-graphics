#
# $Header: /files1/home/toy/libc/src/RCS/Makefile,v 1.2 90/09/10 13:01:23 toy Exp Locker: toy $
#

# Set to "ranlib" if you have it, or "echo" if you don't
RANLIB = ranlib

# Set to "ln" or "ln -s" or even "cp", depending on what you want
# and have
LINK = ln

# How to delete a file
RM = rm -f

# Your favorite compiler and options
CC = gcc -Wall -ansi 
COPTS = -g

# Options for compiling and linking
OPTIMIZE = -O
CFLAGS = $(OPTIMIZE) $(COPTS) $(DEBUG)
LDFLAGS = $(DEBUG)

# Library name and options
LIBRARY = gps-io
LNAME = lib$(LIBRARY).a
LIBOPTS = -L./ -l$(LIBRARY)
LOADLIBES = $(LIBOPTS) $(LM)
LM = -lm

# A rule for making the executables
#.o :
#	$(CC) $(LDFLAGS) -o $* $*.o $(LIBOPTS) $(LM)

# These math programs are actually links to abs
MATH = ceil floor exp cos sin tan acos asin atan gamma

PROGS =	$(LNAME)	\
	cor	\
	cusum	\
	gas	\
	hilo	\
	log	\
	lreg	\
	mean	\
	mod	\
	pair	\
	power	\
	prod	\
	qsort	\
	rand	\
	rank	\
	root	\
	round	\
	siline	\
	total	\
	var	\
	atob	\
	btoa	\
	abs $(MATH)

all : $(PROGS)

atob : atob.o $(LNAME)
	$(CC) $(LDFLAGS) -o atob atob.o $(LOADLIBES)
btoa : btoa.o $(LNAME)
	$(CC) $(LDFLAGS) -o btoa btoa.o $(LOADLIBES)
cor : cor.o $(LNAME)
	$(CC) $(LDFLAGS) -o cor cor.o $(LOADLIBES)
cusum : cusum.o $(LNAME)
	$(CC) $(LDFLAGS) -o cusum cusum.o $(LOADLIBES)
gas : gas.o $(LNAME)
	$(CC) $(LDFLAGS) -o gas gas.o $(LOADLIBES)
hilo : hilo.o $(LNAME)
	$(CC) $(LDFLAGS) -o hilo hilo.o $(LOADLIBES)
log : log.o $(LNAME)
	$(CC) $(LDFLAGS) -o log log.o $(LOADLIBES)
mean : mean.o $(LNAME)
	$(CC) $(LDFLAGS) -o mean mean.o $(LOADLIBES)
mod : mod.o $(LNAME)
	$(CC) $(LDFLAGS) -o mod mod.o $(LOADLIBES)
pair : pair.o $(LNAME)
	$(CC) $(LDFLAGS) -o pair pair.o $(LOADLIBES)
power : power.o $(LNAME)
	$(CC) $(LDFLAGS) -o power power.o $(LOADLIBES)
prod : prod.o $(LNAME)
	$(CC) $(LDFLAGS) -o prod prod.o $(LOADLIBES)
qsort : qsort.o $(LNAME)
	$(CC) $(LDFLAGS) -o qsort qsort.o $(LOADLIBES)
rand : rand.o random.o $(LNAME)
	$(CC) $(LDFLAGS) -o rand rand.o random.o $(LOADLIBES)
rank : rank.o $(LNAME)
	$(CC) $(LDFLAGS) -o rank rank.o $(LOADLIBES)
round : round.o $(LNAME)
	$(CC) $(LDFLAGS) -o round round.o $(LOADLIBES)
siline : siline.o $(LNAME)
	$(CC) $(LDFLAGS) -o siline siline.o $(LOADLIBES)
var : var.o $(LNAME)
	$(CC) $(LDFLAGS) -o var var.o $(LOADLIBES)
abs : math.o $(LNAME)
	$(CC) $(LDFLAGS) -o abs math.o $(LIBOPTS) $(LM)

# These are the targets for programs that are actually
# links to other programs
total : prod
	$(RM) total; $(LINK) prod $@

lreg : cor
	$(RM) lreg; $(LINK) cor $@

root : power
	$(RM) $@; $(LINK) power $@

$(MATH) : abs
	$(RM) $@; $(LINK) abs $@

# Dependencies on header files
abs.o : gps.h
cor.o : gps.h
cusum.o : gps.h
gas.o : gps.h
hilo.o : gps.h
log.o : gps.h
lreg.o : gps.h
mean.o : gps.h
mod.o : gps.h
pair.o : gps.h
power.o : gps.h
prod.o : gps.h
qsort.o : gps.h
rand.o : gps.h
rank.o : gps.h
root.o : gps.h
round.o : gps.h
siline.o : gps.h
total.o : gps.h
var.o : gps.h

# Library

toc =	\
	$(LNAME)(getname.o)	\
	$(LNAME)(getnum.o)	\
	$(LNAME)(message.o)	\
	$(LNAME)(prnum.o)	\
	$(LNAME)(prhelp.o)	\
	$(LNAME)(rdvec.o)	\
	$(LNAME)(setprecn.o)



$(LNAME) : $(toc)
	$(RANLIB) $(LNAME)

$(LNAME)(getname.o) : gps.h
$(LNAME)(getnum.o) : gps.h
$(LNAME)(message.o) : gps.h
$(LNAME)(prnum.o) : gps.h
$(LNAME)(rdvec.o) : gps.h
$(LNAME)(setprecn.o) : gps.h


gps.h :

clean :
	$(RM) $(PROGS) *.o *~


