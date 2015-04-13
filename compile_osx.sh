#!/bin/sh

# This script simplify the compilation of MoriaOP on Mac OSX

# First things first: create the main object file using the free pascal
# compiler
fpc -Mobjpas -O3 -Xs -Cn Moria.dpr

# Then, the link step with ppas.osx, that is, ppas.sh customized for Mac OSX
./ppas.osx

# That's it! MoriaOP should now be in the current directory
