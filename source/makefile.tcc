# /*
#  *
#  * This file, Makefile.dst, is part of the distrat/askmap package
#  * by Jeff Goldberg Copyright (c) 1994, 1995 and is covered by
#  * version 2 (or later, at our option) of the GNU General Public
#  * Software License.  It is free and is distributed with
#  *     NO WARRANTY
#  *
#  */

###########################################################################
##  You may need to change some things in this make file                 ##
##  Those places should be clearly labelled.                             ##
###########################################################################

# This file is for the DOS borland compiler tcc
### Where do you want the files executables to be installed after
### making?  Here you may need to modify what the DEST(intation) bin
### will be.
#
### for dos
DESTBIN=C:\BIN\
#
### for Unix  Your own bin
#DESTBIN=${HOME}/bin
# or some system bin?
#DESTBIN=/usr/local/bin

#### How to install to $(DESTBIN)?
##
## for dos, just copy
INSTALL=COPY
#
# For Unix use either cp or the install program if you will
#INSTALL=cp

#### How to remove file
## DOS
RM=DEL
## 4dos
#RM=del /f
##Unix
#RM=rm -f

#### What C compiler do you want to use?
#CC=gcc
CC=tcc
#CC=cc

### Get the Distrat/Askmap libraray (libdram.{a,lib})
### Lib extension
## Tcc
LE=.lib
## everyone else
#LE=.a
LIBPREFIX=lib
LIBCORENAME=dram
### Library file name
LIBNAME=$(LIBPREFIX)$(LIBCORENAME)$(LE)
### Lib inclusion command
## for tcc
LIBS=$(LIBNAME)
## everyone else
#LIBS= -l$(LIBCORENAME)

#LFLAGS=
LFLAGS=-L.

## Some makes allow things like "V=$(V) Otherstuff", other
## make programs say that should be done with "V+=Otherstuff",
## Others have different mechanisms to get at the same idea.
## Obviously the thing to do would be to put system tests in
## the makefile (like !ifdef __MSDOS__), but each make has
## a different langauge for these as well, so there is no
## way to write a true easy to configure makefile that
## crosses DOS, BSD, ans SYSV.  I suppose that gnumake is
## available for each, but I can't guarentee that everyone
## is using that (I don't have it for DOS).
##
## So, what this means is that instead of doing something
## like "V=$(V) Otherstuff" that way, we will have V1, V2, V3
## and so on.

########## First we do this with CFLAGS:

#### Basic Optimizer and debugging options (gcc allows both -O and -g)
#### others don't.  DOS compilers use very different names.
#
### First the Unix forms
#
## Optimize code (recommend if not using gcc but using Unix)
#CF1=-O
## or leave debugging info
#CF1=-g
## or if you use GCC
#CF1=-O -g

### Now the tcc settings for these
#
## Optimization?
CF1=-O
#
# Source debugging (if you don't know what it is, you don't want it)
#CF1=-v

### Or system default
#CF1=

#### More options (for machine information, unix safe to leave blank)
##
### tcc first
## Use compact model for all programs
#CF2=-mc
## Use compact model and make programs for 286 or better
CF2=-mc -2

### There are some possible Unix things, but these are so machine
### dependent, that it is best to leave with the default unless you
### have very good reasons to add things here.
#CF2=

#### Extra warning messages?  Suppress warning message
### Here are some tcc forms.

## Maximal annoying warning messages (tcc only)
#CF3=-wamb -wdef -wnod -wstv -wuse
## Remove some annoying warning messages (tcc only)
CF3=-w-par -w-rvl
## Or gcc (Unix, DOS)
#CF3=-Wall
### Let the Unix (non-gcc) ones run at default
#CF3=

##### Your own addtions.  If you have more things you would like
##### to add, put them here (Note, if you want to add predefines, do
##### those below)
## CFOTHER= Your Additions
CFOTHER=

##### Predefines  THIS SECTION MATTERS
##### Parts of the program are written to compile differently depending
##### on these values

### If your compiler treats the type of a pointer to anything
## as a (char *) instead of the normal (void *) then use the following line.
## That is, with an old old compiler you will need this  
#PD1=-DANYCHARPTR
PD1=

##  Many things that for dos compilers are in stdlib, are, in
## Unix, in unistd.  One is access().  If your Unix has unistd.h
## then use the following
#PD2=-DHAVE_UNISTD_H
PD2=

#### If there are predefines that you want to use then put them
#### for example.  (If you don't know what these mean (they are
#### specific for compling in defaults) then leave PDOTHER empty)
##### PDOTHER=-DMAXC=100 -DDEF_DB=2
PDOTHER=

###### Shouldn't have to change these two
PREDEFS=$(PD1) $(PD2) $(PDOTHER)
CFLAGS=$(CF1) $(CF2) $(CF3) $(PREDEFS) $(CFOTHERS)


## drsort uses some math functions like fabs() and sqrt() for
## some (tcc) compilers these are in the standard library and there
## is not separate math library.  For others (Unix) you need the math
## library to link.
MATHTLIB=
#MATHLIB=-lm

## Object file extension on this system
## with the dot .obj for tcc, .o for everyone else (I think)
OE=.obj
#OE=.o

## Executable (sort of) file extension (with . if there is one)
EE=.exe
#EE=

## real executable (for djgpp there is extra step for going from
## prog to prog.exe)  Use this for djgpp only.
#EEX=.exe

## How to specify executable name to be created.  For Unix this is
## "-o name" for tcc it is "-ename"
ENAME=-e$@
#ENAME=-o $@

#### GETOPT, if your system provides getopt then use that
## instead of recompiling
GETOPTOBJ= getopt$(OE)
#GETOPTOBJ=

## For djgpp only:
#COFFS=distrat$(EE) iodeg$(EE) askmap$(EE) avmap$(EE)\
#        ross$(EE) drsort$(EE) density$(EE) cmtran$(EE)

## List of things to make with "make all"
TARGETS=distrat$(EE) iodeg$(EE) askmap$(EE) avmap$(EE)\
        ross$(EE) drsort$(EE) density$(EE) cmtran$(EE)

############ You shouldn't have to change anyting below here #############

VCOMOBJ=version$(OE) $(GETOPTOBJ)
COMOBJ=matutil$(OE) readmap$(OE) mapcache$(OE) writemap$(OE) $(VCOMOBJ)

DROBJS=distrat$(OE)
IOOBJS=iodeg$(OE)
AMOBJS=askmap$(OE) amlang$(OE) rossify$(OE)
DENOBJS=density$(OE)
DRSRTOBJS=drsort$(OE)
AVOBJS=avmap$(OE)
TRANOBJS=cmtran$(OE)
ROSSOBJS=ross$(OE) rossify$(OE)

allprogs:	$(TARGETS)

$(LIBNAME):	$(COMOBJ)
#		ar -ru $(LIBNAME)  $(COMOBJ)
		tlib $(LIBNAME) +-version$(OE)
		tlib $(LIBNAME) +-matutil$(OE)
		tlib $(LIBNAME) +-readmap$(OE)
		tlib $(LIBNAME) +-writemap$(OE)
		tlib $(LIBNAME) +-mapcache$(OE)
		tlib $(LIBNAME) +-getopt$(OE)

distrat$(EE):	$(DROBJS) $(LIBNAME)
		$(CC) $(CFLAGS) $(LFLAGS) $(ENAME) $(DROBJS) $(MATHLIB) $(LIBS)

iodeg$(EE):	$(IOOBJS) $(LIBNAME)
		$(CC) $(CFLAGS) $(LFLAGS) $(ENAME) $(IOOBJS) $(LIBS)

askmap$(EE):	$(AMOBJS) $(LIBNAME)
		$(CC) $(CFLAGS) $(LFLAGS) $(ENAME) $(AMOBJS) $(MATHLIB) $(LIBS)

avmap$(EE):	$(AVOBJS) $(LIBNAME)
		$(CC) $(CFLAGS) $(LFLAGS) $(ENAME) $(AVOBJS) $(MATHLIB) $(LIBS)

ross$(EE):	$(ROSSOBJS) $(LIBNAME)
		$(CC) $(CFLAGS) $(LFLAGS) $(ENAME) $(ROSSOBJS) $(LIBS)

drsort$(EE):	$(DRSRTOBJS) $(LIBNAME)
		$(CC) $(CFLAGS) $(LFLAGS) $(ENAME) $(DRSRTOBJS) $(MATHLIB) $(LIBS)

density$(EE):	$(DENOBJS) $(LIBNAME)
		$(CC) $(CFLAGS) $(LFLAGS) $(ENAME) $(DENOBJS) $(LIBS)

cmtran$(EE):	$(TRANOBJS) $(LIBNAME)
		$(CC) $(CFLAGS) $(LFLAGS) $(ENAME) $(TRANOBJS) $(LIBS)

askmap$(OE):	askmap.c lang.h distrat.h

amlang$(OE):	amlang.c lang.h distrat.h

distrat$(OE):	distrat.c distrat.h

mapcache$(OE):	mapcache.c distrat.h

readmap$(OE):	readmap.c distrat.h

writemap$(OE):	writemap.c distrat.h

version$(OE):	version.c distrat.h

avmap$(OE):	avmap.c distrat.h

density$(OE):	density.c distrat.h

ross$(OE):	ross.c distrat.h

drsort$(OE):	drsort.c distrat.h

getopt$(OE):	getopt.c getopt.h

all:		allprogs
		@echo It looks like everything has been made now

clean:
		$(RM) *$(OE) $(COFFS)

vclean:		clean
		$(RM) $(TARGETS) $(LIBNAME)

install:	allprogs
		$(INSTALL) $(TARGETS) $(DESTBIN)

### All targets with $(EEX) make sense with djgpp only.

distrat$(EEX):	distrat$(EE)
		strip $<
		coff2exe $<

iodeg$(EEX):	iodeg$(EE)
		strip $<
		coff2exe $<

askmap$(EEX):	askmap$(EE)
		strip $<
		coff2exe $<

avmap$(EEX):	avmap$(EE)
		strip $<
		coff2exe $<

ross$(EEX):	ross$(EE)
		strip $<
		coff2exe $<

drsort$(EEX):	drsort$(EE)
		strip $<
		coff2exe $<

density$(EEX):	density$(EE)
		strip $<
		coff2exe $<

cmtran$(EEX):	cmtran$(EE)
		strip $<
		coff2exe $<
