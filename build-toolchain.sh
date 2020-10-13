#!/bin/bash

readonly GCC_VERSION=10.2.0
readonly BINUTILS_VERSION=2.35

mkdir -p /tmp/toolchain && cd /tmp/toolchain || exit

if [ ! -f "binutils-$BINUTILS_VERSION.tar.gz" ]
then
    wget "https://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.gz"
    tar zxf "binutils-$BINUTILS_VERSION.tar.gz"
fi

if [ ! -f "gcc-$GCC_VERSION.tar.gz" ]
then
    wget "https://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.gz"
    tar zxf "gcc-$GCC_VERSION.tar.gz"
fi

mkdir -p /tmp/toolchain/build-binutils
mkdir -p /tmp/toolchain/build-gcc

cd /tmp/toolchain/build-binutils && rm -rf ./*
"/tmp/toolchain/binutils-$BINUTILS_VERSION/configure" --target=i686-elf --prefix="$HOME/cross-install" --with-sysroot --disable-nls --disable-werror
make -j4 && make install

cd "/tmp/toolchain/gcc-$GCC_VERSION" && ./contrib/download_prerequisites
cd /tmp/toolchain/build-gcc && rm -rf ./*
"/tmp/toolchain/gcc-$GCC_VERSION/configure" --target=i686-elf --prefix="$HOME/cross-install" --disable-nls --enable-languages=c,c++ --without-headers
make -j4 all-gcc
make -j4 all-target-libgcc
make install-gcc
make install-target-libgcc
