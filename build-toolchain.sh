#!/bin/bash

if [ -n "$(find "$HOME"/cross-install -maxdepth 0 -empty)" ]
then
    mkdir -p /tmp/toolchain && cd /tmp/toolchain || exit

    if [ ! -f "binutils-2.31.tar.gz" ]
    then
        wget https://ftp.gnu.org/gnu/binutils/binutils-2.31.tar.gz
        tar zxf binutils-2.31.tar.gz
    fi

    if [ ! -f "gcc-8.2.0.tar.gz" ]
    then
        wget https://ftp.gnu.org/gnu/gcc/gcc-8.2.0/gcc-8.2.0.tar.gz
        tar zxf gcc-8.2.0.tar.gz
    fi

    mkdir -p /tmp/toolchain/build-binutils
    mkdir -p /tmp/toolchain/build-gcc

    cd /tmp/toolchain/build-binutils && rm -rf ./*
    /tmp/toolchain/binutils-2.31/configure --target=i686-elf --prefix="$HOME/cross-install" --with-sysroot --disable-nls --disable-werror
    make -j4 && make install

    cd /tmp/toolchain/gcc-8.2.0 && ./contrib/download_prerequisites
    cd /tmp/toolchain/build-gcc && rm -rf ./*
    /tmp/toolchain/gcc-8.2.0/configure --target=i686-elf --prefix="$HOME/cross-install" --disable-nls --enable-languages=c,c++ --without-headers
    make -j4 all-gcc
    make -j4 all-target-libgcc
    make install-gcc
    make install-target-libgcc
fi
