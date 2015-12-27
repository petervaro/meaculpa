#!/bin/bash

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
# Constants
HELP="\
\033[1mNAME\033[0m

    install.sh - build and install libmeaculpa

\033[1mSYNOPSIS\033[0m

    install.sh [options]

\033[1mOPTIONS\033[0m

    \033[1m-c\033[0m CC          Specify C compiler
    \033[1m--compiler\033[0m CC  (default: gcc)

    \033[1m-i\033[0m PATH        Specify header install location
    \033[1m--include\033[0m PATH (default: /usr/local/include)

    \033[1m-l\033[0m PATH        Specify library install location
    \033[1m--library\033[0m PATH (default: /usr/local/lib)

    \033[1m-r\033[0m             Uninstall previously installed files
    \033[1m--remove\033[0m

    \033[1m-h\033[0m             Print this text
    \033[1m--help\033m
";
BUILD_DIR="build-install";

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
# Set default values
compiler="gcc";
include="/usr/local/include";
library="/usr/local/lib";
remove="";


#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
# Parse arguments
while [ -n "$1" ];
do
    if [ "$1" == "-c" ] || [ "$1" == "--compiler" ];
    then
        shift;
        compiler="$1";
    elif [ "$1" == "-i" ] || [ "$1" == "--include" ];
    then
        shift;
        include="$1";
    elif [ "$1" == "-l" ] || [ "$1" == "--library" ];
    then
        shift;
        library="$1";
    elif [ "$1" == "-r" ] || [ "$1" == "--remove" ];
    then
        remove="yes";
    elif [ "$1" == "-h" ] || [ "$1" == "--help" ];
    then
        printf "$HELP";
        exit;
    else
        printf "Invalid argument: $1\n";
        exit 1;
    fi;
    shift;
done;


#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
build()
{
    # Compile sources to objects
    mkdir -p $BUILD_DIR/build;
    for source in src/*.c;
    do
        fname=$(basename "$source");
        fname="${fname%.*}";
        $compiler -std=c11 \
                  -O3 \
                  -Iinclude \
                  -lpthread \
                  -fPIC \
                  -o $BUILD_DIR/build/$fname.o \
                  -c $source;
    done;

    # Create both static and dynamic libraries
    mkdir -p $BUILD_DIR/lib;
    $compiler -shared \
              -o $BUILD_DIR/lib/libmeaculpa.so \
              $BUILD_DIR/build/*.o;
    ar rcs \
       -o $BUILD_DIR/lib/libmeaculpa.a \
       $BUILD_DIR/build/*.o;
}


#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
if [ -n "$remove" ];
then
    printf "Uninstalling libmeaculpa...\n";
    sudo rm -Rf $include/meaculpa;
    sudo rm -f $library/libmeaculpa*;
    printf "libmeaculpa successfully removed\n";
else
    # Build library
    printf "Building libmeaculpa...\n";
    build
    printf "libmeaculpa successfully built\n";

    # Install library
    printf "Installing libmeaculpa...\n";
    sudo cp -R include/meaculpa $include;
    sudo cp $BUILD_DIR/lib/libmeaculpa* $library;
    printf "libmeaculpa successfully installed\n";
fi;

exit;
