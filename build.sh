#!/bin/bash

if [ -d ./build ];then
    rm -rf ./build/
fi

if [ $EUID -eq 0 ];then 
    apt update && apt install -y cmake make clang libssh-dev libcurl4-gnutls-dev 
fi

mkdir build;
cmake -S ./ -B ./build;
cd build; 
make
chmod +x ./gau8