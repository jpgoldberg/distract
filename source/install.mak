  *** THIS DOCUMENTATION IS PART OF THE distrat/askmap PACKAGE.
  *** Distribution is subject to the terms of the General Public
  *** Software License (version 2 or later).  There is NO WARRANTY.
  ***
  (c) 1994,1995 Jeff Goldberg


	MODIFYING THE MAKEFILES AND COMPILING

If you are not installing executables already created for your system,
either because the distribution does not have such executables, or
because you would like your own specially configured version, or
for safety reasons (A quick glance through the source files will
convince you there are no viruses, etc), or any number of these reasons,
you will need to compile the sources.

If you are familier with make and C then you should just need to
look at the section "PICKING A MAKEFILE", and in the event of errors
the section on possible "IF THERE WERE PROBLMES WITH MAKE".  Each
of these section titles begins at the begining of a line if you
want to quickly search this file.

WHAT YOU NEED

In addition to the files in source directory for this distribution you
will need a

    C compiler that does ANSI for your type of machine.  gcc is
    recommended

    A linker/loader.  (If you have a C compiler you will have a loader)

    The "make" utility.  (Not actually necessary, but this documentation
         assumes that you have it.)

    The command "touch" would be useful (with make), but you can easily
    do without it.

[Note you won't need these tools to run the executable programs,
only to make them.  So, if you don't have a C compilier you could
compile the code on a machine that does (as long as it produces the
right kind of executables for your machine).]
  
A FEW WORDS ABOUT MAKE

If you are unfamiliar with make, it is worth understanding basically
what make does.  If you do know about make, then just skip this
section.

Make reads a special file (called a Makefile) which lists what files
depend on what, and how to make the former.  For example, suppose
that you worked in a text editor that created files in a particular
form (lets say .XXX), but for other purposes you sometimes needed the
file in RTF form (rich text format).  Suppose that your editor didn't
have a way of saving in RTF, but that you had a program, called
xxx2rtf which did the translation.  If this case, the .rtf file (the
target) depends on the .xxx file, and the makefile would have a rule
like

foo.rtf:	foo.xxx
		xxx2rtf foo

If this were the case, then when you typed the command, "make foo.rtf",
then make would check to see if the file foo.rtf already exists, if
it does it would check to see whether the file foo.rtf as been modified
more recently then the file foo.xxx.  If so, make would do nothing
because foo.rtf is upto date.

If on the otherhand, foo.xxx has been modified more recently (or is
not up to date), then make would run the command  "xxx2rtf foo".  If
foo.xxx depends on other things (as possibly stated elsewhere in the
makefile), then make would first try to `make foo.xxx' and only after
foo.xxx is up to date would it run the command to make foo.rft from
foo.xxx

An almost real life example:

I use the typesetting system TeX.  The program TeX reads in a file
which is usually created by a person and produces something called a
dvi (DeVice Independent) file.  To print the .dvi file, one needs to
translate from that to the kind of file that the printer wants.
Let's say an .hp file for printers that use PCL (Hewlett-Packard
compatible Printer Control Language).  In this situation one could
have a makefile with the following information:

foo.dvi:	foo.tex
		tex foo.tex

foo.hp:		foo.dvi
		dvi2hp foo

So if I edit foo.tex and want to produce the .hp file, I would just
type "make foo.hp".

Things can get more complicated.  Sometimes I don't what to type in
the tex file directly, but have it produced by a program that reads
something that I do type in and translates that to TeX.  There is
a program that will automatically put in the tex commands for Hungarian
hypenation so the user doesn't have to use a special version of tex
for Hungarian.  This program, hion, takes files and .hun and makes
.tex files.  In this case it is the .hun file that the human edits.
So the makefile would look like:

foo.dvi:	foo.tex
		tex foo.tex

foo.hp:		foo.dvi
		dvi2hp foo

foo.tex:	foo.hun
		hion -TEX foo.hun

Of course A tex file can input other tex files, so sometimes the .dvi
file depends on more than one file.

foo.dvi		foomain.tex table1.tex figure1.ps appendix1.tex
		tex foomain.tex
		rename foomain.dvi foo.dvi

And there could be all rules about making the things that foo.dvi
depends on.

Two more things about make.  First it is possible to teach make how
to make things based on the file extensions instead of the full file
names.  So for example, it would be possible to tell make that any
file NAME.dvi can be made from any file NAME.tex by running "tex
NAME.tex".  I won't explain how.  Some of these sorts of rules are
"builtin" to make.  So every make knows generally how to get from a
file "NAME.c" to "NAME.o" (or "NAME.obj" on DOS).

The second other thing about make, is that you can define variables
So, in the above, where I have "rename" as part of the command for
renaming the file foomain.dvi to foo.dvi, I could have had something like

RENAME=ren

...
		$(RENAME) foomain.dvi foo.dvi

Lines in a make file that begin with a "#" are not processed by make;
So a makefile could have

#### use the ren for DOS
RENAME=ren
#### or mv for unix
#RENAME=mv

...
	$(RENAME) foomain.dvi foo.dvi

If none of the Makefile templates that are provided with this
distribution work exactly for your system, then you will have to
change some of these sorts of things for your system, eg, from the
above to

#### use the ren for DOS
#RENAME=ren
#### or mv for unix
RENAME=mv

...

See the difference?  (the second uses mv, while the former will use
ren).

PICKING A MAKEFILE

There are several makefiles provided, and with luck, one will work
for your system without modification.  The file makefile.dst is
the general one that all of the others are modelled from.  It will
NOT work on any system.

	Makefile.tcc	TurboC on DOS.  Use this as a model for
			other compilers on DOS.

	Makefile.dgp    For DOS gcc (djgpp).  This file has
			an extra layer for the coff2exe
			stage needed by djgpp.

	Makefile.gcc	Standard Unix with gcc (Recommended)

	Makefile.unx	Standard Unix, but using cc

	Makefile.dst	Template makefile.  Will NOT WORK on any
			system.

What you should do, copy the file that seems the most reasonable to
you to the file Makefile :

% cp Makefile.xxx Makefile
or
C:> copy makefile.xxx makefile

And just try it:

% make
or
C:> make

If all goes well, you should have the eight executable programs

askmap
avmap
cmtran
density
distrat
drsort
iodeg
ross

(or with the extension .exe on DOS)
You can copy these to where every you want them, or you can use

% make install
or
C:> make install

But, before using make install, you should look at the Makefile
and see that DESTDIR is set up the way you want it.  (and that
the directory exists on the system).  So you might have to
edit the make file for make install.

IF THERE WERE PROBLEMS WITH MAKE

What you need to do depends on the program, so you will need to read
the output of make carefully.  Note that make also displays the
commands that it calls.  This way you can see how far make got.

Error type:   Out of memory

    You are obviously using DOS.  See the section on special
    instructions for DOS.

Error type:  blah blah Command not found
    
    If this is something like gcc command not found, then it is likely
    that the named C compiler from the makefile does not exist on
    your system.  The makefile has a section for selecting the compiler.
    You will need to make changes in that section.
    
    If you are on Unix you can safely use "CC=cc".  On DOS you will
    need to know what the name of your compilier is.  You may need to
    change many things in the DOS version if you are not using tcc.   
	
Error type:  file limits.h not found

    This error might happen during a compile (probably
    of the file rossify.c).  It means that you don't have
    a properly configured C compiling environment.  You can get around
    this particular problem by setting "-DNOLIMITS" in the "Predefineds"
    section of the Makefile.  But chances are, if this has gone wrong
    other things will go wrong as well.

    You should safely be able to restart the make, without having
    to remake the other things already made.

Error type:  parse error, or syntax error, or bad cast

    If you follow the output of make carefully you should be able
    you should be able to see what file the compiler was working
    on.  The error message from the compiler will say what line
    number the error occurred on (or near).  It may also give
    a number for the error, so be sure to guess that you are looking
    at the line number and not the error number.

    Look at the line in the appropriate file.  Don't worry, you won't
    have to change anything in the source code.  This is just to see
    what kind of error it is.

    If on or near that line there is "ANYPTR", then you will should
    edit the predefines in the Makefile to use "-DANYCHARPTR".  Chances
    are that if you have encountered this error you will also encounter
    the more serious errors described next.  Anyway, it won't hurt to make
    the change to the makefile and try again.

    If on or near that line was something like

blah blah
function(blah blah, blah blah)
{
    It doesn't matter what the `function' or `blah' is, what matters
    is that in the "(" and ")" there are (at least) two `blah's between
    any commas or the the parenthesis stuff looks like "(void)".  Also
    this will always be at the begining of a line.

    If that is what you found, it means that your C compiler doesn't
    understand prototypes in function definitions.  If this is what
    happened then, at the moment there is nothing to do, but try a
    more modern C compiler.  Definitly let me know about this, because
    I may rewrite the program with a NODEFPROTOTYPES to deal with this
    if there are still many old  C compilers around, but I would rather
    not have to.  Try to find another C compiler, or see if your
    compiler has something like an "ansi" option or something like that.

    In particular if you are compiling on a Sun workstation.  Do
    not use cc, but use gcc or acc.  There is a tool called
    unproto which will fix to work with obsolete C compilers.

    If the syntax error was in the file distrat.h and near a line that
    looks like

extern blah function(blah blah)

    then your compiler doesn't even understand prototypes in function
    declarations.  Again, if you have this problem then you probably have
    the above problems as well.  Try another C compiler.

REPORTING PROBLEMS

Naturally, I would be interested in knowing about problems you
encounter, but my curiosity does not entail a commitment to
fix anything.  But it is certainly more likely that I will be
able to fix things if you give me as complete information as
you can.  Tell me about your system. (on Unix the "uname -a"
command will tell you.  About your compiler (for gcc the command
"gcc -v" will tell you).  Most importantly, I need the error
messages.  Please run make putting all make out put into
a file, say "make.out".  On DOS do this with

C:> make all > make.out

On Unix, if you are using csh or some derivative of it like tcsh
or mitcsh use

% make all >& make.out

Use the & to get the error messages there as well.  If you are using
the Bourne (standard) shell (sh) or the things like
it (bash, Bourne Again SHell) or the Korn Shell (ksh)

$ make all >make.out  2>&1

If you don't know which type of shell you are using, type

% csh
% make all >& make.out
% exit

This way, you will use the csh.  Also

% echo $SHELL

will tell you.

If for some inexplicatable reason you are using Bill Poser's jsh
then I think the command should be

% all make.out >& make

(Object--Postpositional phrase--verb order!  Jsh uses Japanese word
order)


SPECIAL INSTRUCTIONS FOR DOS

    If you have never used make or a compiler on your machine, it
    is really best for you to use the assistance of someone who
    is familiar with these tools and DOS, since you often need to
    mess around with memory managers to get make and the C compiler
    to run.  (The distrat/askmap programs do not need any special
    memory manager to run, it is make, the linker and the compiler
    that need extended or expanded memory or use special memory managers.)

    As an warning, I just attempted making this with exactly the same
    compiler and make on Livia's machine which has sufficient RAM, but
    it turns out that the DOS utility Smartdrv was installed in such a
    way it took too much memory, so make wasn't getting enough to be
    able to it on the the linker.  It took me several hours one day,
    a night's sleep, and an hour the next to identify the problem.  Once
    identified all worked well.  (A DOS expert would probably figure it
    out much faster).

    The novice should feel free to try to compile under DOS, but if
    you get an out-of-memory error during compilation, consult a
    local DOS programer.  And when you start cursing, that cursing
    should be directed not at me, but toward Seattle.  The problems
    aren't my fault nor the fault of the developers of the C compiler
    (unless, of course you are using mcc, but that's just guilt by
    association.)

