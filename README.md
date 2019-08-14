# Bare Bones i386 kernel

This is a restored back-up that's about 300~ commits behind.

## Prerequisites

This project uses [standardese](https://github.com/foonathan/standardese) for documentation. Please use the link
for instructions on obtaining it. The easiest way is to use Docker:

```
docker pull foonathan/standardese_dev
docker run -v "/path/to/standardese/source:/root/standardese" -v "$(pwd):/root/output" foonathan/standardese_dev
```

## Building

You have two options. You can either use Docker or the `build-toolchain.sh` script
to get the `i686-elf` cross toolchain on your system.

### Docker

Grab my [`i686-cross-gcc` image](https://github.com/remyabel/i686-cross-gcc-docker):

```shell
docker pull remyabel/i686-cross-gcc
```

Then simply mount this repo and build it:

```shell
docker run -it --user $(id -u) -v "$PATH_TO_REPO":/app remyabel/i686-cross-gcc /bin/sh -c "mkdir -p /app/obj; make -C /app main"
```

### Local toolchain and make

If you choose not to use Docker, just run `build-toolchain.sh`. GCC and Binutils will be located in
`$HOME/cross-install`, so add it to your path. Then run `make main`. You can also use CMake like so:

```shell
env CROSS_GCC_PREFIX=$HOME/cross-install/ cmake -DCMAKE_TOOLCHAIN_FILE=$PWD/elf-toolchain.cmake -H. -Bbuild-output
cmake --build build-output -- -j4
```

## Running the kernel

Once you have everything built, run these two commands:

```shell
make iso
make qemu
```

This will create an ISO from the binary and open it up in Qemu.

## Running the tests

Building the tests require CMake and Conan (to grab the Catch2 library). Use the following commands:

```shell
cd test
conan install -if build-output .
cmake -H. -Bbuild-output 
cmake --build build-output -- -j4
./build-output/bin/cpp-i386-kernel-tests
```

Double check that your conan profile uses the correct GCC version.
