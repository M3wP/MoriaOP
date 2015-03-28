----------------------------------------------------------------------------
                          M O R I A   5 . 0 2

                  COPYRIGHT (c) Robert Alan Koeneke
            Ported to Delphi/Free Pascal by Daniel England
----------------------------------------------------------------------------

Introduction
------------

Hello and welcome to Moria!

This is a port of the game Moria version 5 from DEC/VMS (the exact lineage
is difficult for me to determine).  Unlike other Moria versions and their
descendants, this version is still in Pascal like the original and is not based
on a translation into C.  Also, the other games were based on the older version
4.8.  Being written in Pascal, it is easy to compile and modify and with
Free Pascal, has the potential of running on a large number of platforms without
any or perhaps only minor changes.


What is it?
----------

Moria is a hack-and-slash, turn based, role-playing game based on the game
Rogue.  If you've heard of Angband, Moria is the predecessor of that game.

Moria is quite simple to play.  Simply "roll" a character that matches
how you want to play and scour the dungeons for items and gold, killing
nasty creatures with weapons and/or magic along the way.  The objective of
the game is to reach the bottom and slay the vile Balrog.


Features
--------

This port will compile with Delphi (XE2/XE4/XE7 tested only at the moment) or
with Lazarus/Free Pascal on Windows, Unix/Linux, Mac OSX and probably other
platforms too (like Amiga and Atari) but it hasn't been tested.

A number of bugs were fixed (most notably the racial searching ability) and
vital feedback can be given through the use of colour.

A much larger light radius has been implemented.  The original only allowed
you to see up to one square (or what the game called 10 feet) in any
direction. Thanks to Michał Bieliński, this has been increased to 5 squares
(or 50 feet).  Infra vision distances have been doubled.  This increase in
the  visible range may change for the upcoming difficulty settings
modification.

Save games seem to be transportable between the various platforms.


Limitations
-----------

The game is quite difficult to complete.  This should be relaxed in another
release with support for three difficulty settings and a hardcore/softcore
mode selection.

The colour support can be a little shaky, depending upon the platform and
terminal application used.  A configuration setting allows you to determine
how Moria should handle colour for your system.

Save games are handled much like the original.  This means that you must
make sure that you save the game before you quit or your character will be
lost.  The save file is deleted when you start the game just like the
original.

The port is not designed to run on a server like the original was.
Availability hours functionality, user preventions and some save game
features have been dropped for simplicity.

The documentation seems to be out of date.  You cannot launch the extended
help pages from within Moria (so pressing '?' for help sometimes does not
work).


Configuring
-----------

You may change the settings in config.ini to tailor the way in which Moria
behaves.  Currently, there are two settings supported.  One for screen mode
(black and white, eight colour or sixteen colour) and one for cursor size
(normal or large).

You will need to set screen mode to black and white or eight colour for the
standard terminal on Unix/Linux since it doesn't support the AIX high
intensity colour extensions.  However, sixteen colour has been tested on
XTerms and GUI Terminal applications on Linux and Mac OSX and works.

Sixteen colour also works on Windows.  Eight colour is handled as per DOS
on Windows giving sixteen foreground colours and eight background colours
since this platform does not support bold text.

The cursor does not seem to blink on XTerm like on other terminal apps and
looks odd for not allowing proper feedback from the colours used.  I
recommend setting the cursor size to normal if you play Moria in an XTerm.

Read config.ini for details on how to set specific values.


Usage
-----

    Moria [<SAVE FILE>]
    
Simply run Moria (or Moria.exe on Windows) with or without an existing save
game name.  On Unix/Linux and OS X, you will need to run the program from a
terminal window and use the "./" launching technique (unless it is in your 
path).  

After rolling your character (follow the prompts), the game screen is 
displayed.  Your player is represented by the '@' symbol.  Use the numeric 
keypad (with "num lock" on) to move around.  To save use Control+Z and enter 
a save file name.  To quit use Ctrl+Y.

You can press '?' while on the game play screen for a list of available 
commands.  See the file instructions.txt for further details about the game. 
You can also press '/' and then any character to get information about what
that character represents.


Compiling
---------

To compile Moria, you will need a Pascal compiler.  Delphi will do for the
Windows platform but currently, only XE2 and XE4 are supported.  The better 
option is to use Free Pascal, which supports a large number of platforms and 
is free.

You can get the Free Pascal Compiler for most platforms from:
    
    http://www.freepascal.org/download.var


Compiling - for Windows and Linux
---------------------------------

Extract the source files to a location on your disk.

With Delphi XE2 or XE4, simply load Moria.dproj and compile.

With Free Pascal from a console (terminal) window, browse to the 
folder/directory in which you placed the files and type:

    fpc -Mobjpas -O3 -Xs Moria.dpr


Compiling - for MacOSX
----------------------

Compiling on MacOSX is a bit more involved, at least on Lion (10.7+)
The main problem is that Apple has broken the backward compatibility of the 
CRT libraries, that is, the interface between the game and the terminal 
screen. So, you need to use three simple steps as a workaround.

1) Compile Moria. This is done (assuming you have fpc installed) with 
this command:

    fpc -Mobjpas -O3 -Xs -Cn Moria.dpr

Note the additional parameter -Cn. This instruct the compiler not to
build the executable yet.  Instead, it makes fpc create a script, ppas.sh, 
in the current dir.

2) Edit ppas.sh with your favourite editor (it's just a regular text file, 
it won't hurt your system).  For convenience, it is reported here, 
between the lines that starts with ----- (*not* present in the original 
file!):

    ---- here starts ppas.sh ----
    #!/bin/sh
    DoExitAsm ()
    { echo "An error occurred while assembling $1"; exit 1; }
    DoExitLink ()
    { echo "An error occurred while linking $1"; exit 1; }
    echo Linking Moria
    OFS=$IFS
    IFS="
    "
    /usr/bin/ld /usr/lib/crt1.o     -x -multiply_defined suppress -L. -o Moria `cat link.res` -pagezero_size 0x10000
    if [ $? != 0 ]; then DoExitLink Moria; fi
    IFS=$OFS
    --- here ends ppas.sh ----

See the 0x10000 near the end of the file?  All you need to do now is 
after that, add the following (another command argument): 
    
    -macosx_version_min 10.5

Include a space before it, of course.  In other words, ppas.sh should 
now look like this:

    ---- here starts modified ppas.sh ----
    #!/bin/sh
    DoExitAsm ()
    { echo "An error occurred while assembling $1"; exit 1; }
    DoExitLink ()
    { echo "An error occurred while linking $1"; exit 1; }
    echo Linking Moria
    OFS=$IFS
    IFS="
    "
    /usr/bin/ld /usr/lib/crt1.o     -x -multiply_defined suppress -L. -o Moria `cat link.res` -pagezero_size 0x10000 -macosx_version_min 10.5
    if [ $? != 0 ]; then DoExitLink Moria; fi
    IFS=$OFS
    --- here ends modified ppas.sh ----

The additional parameter tells to the computer to use the old version of
the CRT library.  Save and exit.

3) Now run the (modified) ppas.sh script in the terminal with:

    ./ppas.sh

And you're done! This final step creates the executable Moria that is the
one you need to launch to play.

Thanks to Francesco Tortorici for these instructions.


Planned Features
----------------

- Full HTML documentation and launching from in game.
- Better character generation (with a points pool instead of just random 
  numbers.)
- Difficulty settings.


Conclusion
----------

I hope you enjoy playing this version of Moria.  If you have any comments or
suggestions or if you find a bug, please contact me at:
    
    mewpokemon@hotmail.com
    
Please use the word "Moria" in the subject line or your e-mail may get lost.




Daniel England.
