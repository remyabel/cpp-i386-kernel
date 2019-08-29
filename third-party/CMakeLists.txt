# This file is part of cpp-i386-kernel.

cmake_minimum_required(VERSION 3.14.5)

project(libgcov-embedded C)

set(PROJECT_PREFIX "libgcov-embedded/port")

add_library(libgcov_gcc OBJECT ${PROJECT_PREFIX}/gcc/gcov-io.c)
target_include_directories(libgcov_gcc
                           PRIVATE ${PROJECT_PREFIX}/libgcc
                                   ${PROJECT_PREFIX}/gcc/ ${PROJECT_PREFIX}/src)
set_target_properties(libgcov_gcc PROPERTIES LINKER_LANGUAGE C)
target_compile_definitions(libgcov_gcc PRIVATE "ENABLE_LIBGCOV_PORT=1")
target_compile_options(libgcov_gcc PRIVATE "-ffreestanding" "-nostdlib")

add_library(libgcov_libgcc OBJECT
            ${PROJECT_PREFIX}/libgcc/libgcov-driver.c
            ${PROJECT_PREFIX}/libgcc/libgcov-driver-system.c)
target_include_directories(libgcov_libgcc
                           PRIVATE ${PROJECT_PREFIX}/libgcc
                                   ${PROJECT_PREFIX}/gcc/ ${PROJECT_PREFIX}/src)
set_target_properties(libgcov_libgcc PROPERTIES LINKER_LANGUAGE C)
target_compile_definitions(libgcov_libgcc PRIVATE "ENABLE_LIBGCOV_PORT=1")
target_compile_options(libgcov_libgcc PRIVATE "-ffreestanding" "-nostdlib")

add_library(libgcov_src OBJECT ${PROJECT_PREFIX}/src/libgcov-embedded.c)
target_include_directories(libgcov_src
                           PRIVATE ${PROJECT_PREFIX}/libgcc
                                   ${PROJECT_PREFIX}/gcc/ ${PROJECT_PREFIX}/src)
set_target_properties(libgcov_src PROPERTIES LINKER_LANGUAGE C)
target_compile_definitions(libgcov_src PRIVATE "ENABLE_LIBGCOV_PORT=1")
target_compile_options(libgcov_src PRIVATE "-ffreestanding" "-nostdlib")
