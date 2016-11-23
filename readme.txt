Last Changed:  Feb 3, 1996

This is for the distrat/askmap package of programs for
analysis of causal maps.  It is distributed freely and
WITHOUT WARRANTY under the terms of the General Public
License version 2 (or later) (see the file COPYING).
     
Files:

NEEDED	  NAME          Aprx Size       DESCRIPTION
(see note)

?         readme.txt	  9K		This file
Yes	  drdist.zip    945K		DOS executable, docs, sources
1	  unzip.exe      44K		Extract from .zip file
OR
Yes       drdist.tgz
	(or  drdist.tar.gz
	or drdist.taz)	460K		Source and documentation

-------
Yes:  Absolutely needed for the package of programs.  Take either
   dridist.zip (an info zipfile with DOS executables) or drdist.tgz
   a tar'ed gzip file without executables).

?: If you need to read this, then you need to read that.
1: A DOS executible of the info unzip program[*].  You probably
   already have this (or maybe pkunzip which will also work, but
   info (un)zip is free and available with source while pk(un)zip is
   shareware).

Both the .zip file and the .tgz file (as well as the .exe file) need
to be transfered in binary mode.  If you are using ftp, make sure to
specify the type as binary for all transfers involving compressed
files.

      EXTRACTING THE FILES FROM THE ARCHIVE FILES

For the .zip file
-----------------

Use either unzip (InfoZip) or PKunzip to extract from the archive.  If you
use PKunzip then make sure to use the -d option for creating subdirectories.

Assuming that you will use the unzip provided and that everything is on
a floppy in drive "a:" and that you wish to install things on "c:" drive,
do the following:
  c:
  cd \
  a:unzip a:drdist.zip

This will build a directory C:\distrat which in turn has three subdirectories.
Go into the directory and read C:\distrat\doc\install.dos.  Or see about
printing the complete documentation below.

If you use unzip on something other than DOS, be sure to use the -a option
so that the ASCII files are unziped in ASCII mode with newline properly
translated.


For the .tgz file
-----------------

For the .tgz archive, you will need the generally available
programs: gzip (or gunzip), and tar.  Or a version of tar which
incorporates gzip.

  gzip is available by ftp from prep.ai.mit.edu:/pub/gnu
  A free version of tar is also available from the same
  source.  The versions of gzip and tar included here
  are covered by the GNU General Public License agreement and you
  have the right not only to the executables I have included above
  but to the sources.

  tar also exists on every Unix machine, and
  on most other mutliuser systems.  It is possible to gunzip
  and un-tar the files on such a machine and then transfer
  the files to their final destination if that destination does
  not have the suitable programs.

With a version of tar that knows about gzip files: use
 
   tar -zxvf file.tar.gz
or
   tar -zxvf file.tgz

to extract the files from the gzip'ed archive.  Such a version
of tar may be called gtar on some systems.  On DOS tar will
not be able to call gunzip.

If your tar does not understand such things (such as tradition
versions of tar or on DOS), you will need to first gunzip the files
and then extract with tar.  One way of doing this is  

    gunzip file.tar.gz
or
    gunzip file.tgz

which will leave you with a file called "file.tar".

If you have gzip but not gunzip, use "gzip -d" in place of "gunzip".
They are the same thing.

Then

    tar -xvf file.tar

DOS executables for unzip.exe, gunzip.exe, and tar.exe should be
near by this file in the archive.

   LINE FEEDS AND CARRIAGE RETURNS

Different systems deal with line feeds differently.  On DOS
a newline is a sequence of <CTRL-M><CTRL-J> on Unix it
is just a <CTRL-J>.  This .zip file is created with ASCII
files having DOS newlines, and the .tgz file has Unix linefeeds.
But, if you use the -a option to unzip on Unix, you will get
the correct line feeds.

Please direct questions to Jeff Goldberg <J.Goldberg@Cranfield.a.uk>

   THE DIRECTORIES

(I use "/" as the directory separator below)

distrat/doc  Contains documantation and a few sample files
distrat/source contains everything needed to rebuild the executables
distrat/bin contains DOS AT (286) executables, which run just fine
	    on 386s and above.  This directory also contains the .aml
	    files used by askmap
distrat/386 Contains 386 DOS executables and go32.exe (32bit memory
	    extender, which must be placed in your PATH if you
	    wish to use these versions).  These 386 versions can
	    make use of all free memory on your machine.  If you
	    wish to use these, copy of .aml files from the distrat/bin
	    directory to the appropriate place.


   PRINTING THE REAL DOCUMENTATION

There is a largely complete rewrite of the documentation in a much
more coherent form than in the individuals .txt files in the doc
directory.  The new documentation is in four forms:

     drdoc.tex		 LaTeX source (not really usable since
			 many specialized macros are not included)

     drdoc.dvi		If you know that this is, use it, otherwise,
			you can very safely delete it.

     drdoc.ps		PostScript file (300dpi)

     drdoc.hp		PCL (HPlaserJet) file

The documentation is approximately 70 pages long.

Since I have received few requests for the software from North America,
these documents are produced for A4 paper.  North Americans can
still print them out on letter paper, but the margins will be funny.

To print these out you need to determine three (and slightly more) things:

    (1)  What Operating system are you using (I will discuss only
	 DOS and Unix below.  If someone can provide information for
	 Macs, I would be more than happy to include that here.

    (2)  What kind of printer you have.  You will need either a printer
	 Which understands PostScript (A printer language) or
	 which understands PCL (another printer language), which
	 is the language used by HP LaserJets and compatibles.  If
	 you have a choice between the two, use the PostScript printer.
         If you only have a postscript printer, then you can remove
         the .hp file, and if you only have access to a PCL printer
	 you can remove the .ps file.

    (3)  The name of the printer.  This may be something like "lpt2:".

	 DOS users may encounter other problems which I can only
	 half anticipate, since there are too many ways things can
	 be set up at your end.

Printing On DOS:

Do NOT try to load either the .hp or .ps file into some sort of text processor
or run them through a printer driver.  They are already prepared to go
straight to a printer without the intervention of any software.  That
is, the files are already in exactly the form that printers expect information
to be in.

If you have a PCL printer on lpt1, then get to a DOS prompt and go
into the distrat/doc directory, then issue the command

C:> copy/b drdoc.hp lpt1:

Note the "/b"; it is needed

If you have a PostScript printer (preferred) on, say, lpt2: then copy
the PostScript file directly to the printer

C:> copy drdoc.ps lpt2:

(Note that here is there is no "/b")

Special Problems with DOS.

Depending on how your system is designed to talk to the printer, you may
get an error message every few pages (depending on the size of the printer's
memory).  Keep hitting "retry" when prompted to.  If this doesn't
work, please ask someone local for help.

Printing on Unix

If you have a PostScript printer (or print queue) called "fred_ps", 
then just use:

% lpr -Pfred_ps drdoc.ps

If you have a PCL printer (or queue) called "fred_laser",

% lpr -Pfred_laser drdoc.hp

If you have problems printing these, take the files, and a printed
out version of this "readme.txt" file to someone who knows your
system for more help.  If that person needs to contact me with
more questions they should feel free.  I may be able to help
you remotely, but too much of how to print is specific to how
your system is set up.

----- 
*The executable version of unzip.exe, tar.exe, gzip.exe are covered
by the GNU General Public License Agreement (see the file COPYING
among the files extracted).  You have the right to the source code
for that program, which, unless I provide them to you, or ensure that
they appear on the same archive as the executable files, I am in
minor violation of the License Agreement.  I will attempt to remedy
this as soon as I can.  The source code is available from other
archives (namely via ftp from prep.ai.mit.edu in the directory
/pub/gnu).
