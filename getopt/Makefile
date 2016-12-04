# Makefile for the GNU getopt library.
# 
# Copyright (C) 1990 Free Software Foundation, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 1, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
CC = gcc
CFLAGS = -g -Wall -I../include
.PHONY: all clean realclean

all : getopt.o getopt1.o

getopt.o : getopt.c getopt.h 
getopt1.o : getopt1.c getopt.h 

clean :
	-rm -f *.o *~ \#* core test* TAGS
realclean : clean

install :
