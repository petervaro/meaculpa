#!/bin/bash

if [ "$1" == "remove" ];
then
    printf "Uninstalling libmeaculpa...\n";
    sudo rm -Rf /usr/local/include/meaculpa;
    sudo rm -f /usr/local/lib/libmeaculpa*;
    printf "libmeaculpa successfully removed\n";
else
    printf "Installing libmeaculpa...\n";
    sudo cp -R include/meaculpa /usr/local/include;
    sudo cp build-clang-release/lib/libmeaculpa* /usr/local/lib;
    printf "libmeaculpa successfully installed\n";
fi;

exit;
