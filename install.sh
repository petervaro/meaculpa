#!/bin/bash

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
build()
{
    # Set compiler
    if [ "$1" == "clang" ];
    then
        CC=clang;
    else
        CC=gcc;
    fi;

    # Compile sources to objects
    mkdir -p build-install/build;
    for source in src/*.c;
    do
        fname=$(basename "$source");
        fname="${fname%.*}";
        $CC -std=c11 \
            -O3 \
            -Iinclude \
            -lpthread \
            -fPIC \
            -o build-install/build/$fname.o \
            -c $source;
    done;

    # Create both static and dynamic libraries
    mkdir -p build-install/lib;
    $CC -shared -o build-install/lib/libmeaculpa.so build-install/build/*.o;
    ar rcs -o build-install/lib/libmeaculpa.a build-install/build/*.o;
}


#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
if [ "$1" == "remove" ];
then
    printf "Uninstalling libmeaculpa...\n";
    sudo rm -Rf /usr/local/include/meaculpa;
    sudo rm -f /usr/local/lib/libmeaculpa*;
    printf "libmeaculpa successfully removed\n";
else
    build_dir=build-clang-release

    # If this is not dev build
    if [ ! -f $build_dir ];
    then
        printf "Building libmeaculpa...\n";
        build $1;
        build_dir=build-install;
    fi;

    # Install library
    printf "Installing libmeaculpa...\n";
    sudo cp -R include/meaculpa /usr/local/include;
    sudo cp $build_dir/lib/libmeaculpa* /usr/local/lib;
    printf "libmeaculpa successfully installed\n";
fi;

exit;
