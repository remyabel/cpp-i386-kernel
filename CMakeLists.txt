cmake_minimum_required(VERSION 3.14.5)

project(Cpp-i386-kernel CXX C)

enable_language(ASM_NASM)

set(CMAKE_CXX_STANDARD 17)

option(ENABLE_COVERAGE "Enable coverage build." OFF)

if(ENABLE_COVERAGE)
    message(WARNING "The coverage build is currently not functional. Disabling.")
    set(ENABLE_COVERAGE OFF)
endif()

find_program(CLANG_TIDY_PROGRAM
             NAMES "clang-tidy"
             DOC "Path to clang-tidy executable")
mark_as_advanced(CLANG_TIDY_PROGRAM)
if(NOT CLANG_TIDY_PROGRAM)
  message(STATUS "clang-tidy not found.")
else()
  message(STATUS "clang-tidy found: ${CLANG_TIDY_PROGRAM}")
endif()

# The set of headers we want to apply clang-tidy to. We are explicit here
# because it is difficult (if not impossible) to exclude headers with clang-
# tidy.
set(headers
    include/multiboot2.hpp
    include/gdt.hpp
    include/isr.hpp
    include/kstd/algorithm.hpp
    include/kstd/basic_string_view.hpp
    include/kstd/string.hpp
    include/kstd/array.hpp
    include/kstd/iterator.hpp
    include/kstd/utilities.hpp
    include/kstd/char_traits.hpp
    include/inline_asm.hpp
    include/serial.hpp
    include/idt.hpp
    include/terminal.hpp
    include/internal/terminal_helpers.hpp
    include/terminal_iterator.hpp
    include/string.h)
list(JOIN
     headers
     "|"
     joined_headers)

function(add_clang_tidy target)
  if(CLANG_TIDY_PROGRAM)
    set_target_properties(
      ${target}
      PROPERTIES CXX_CLANG_TIDY
                 "${CLANG_TIDY_PROGRAM};-header-filter='${joined_headers}'")
  endif()
endfunction()

function(add_coverage_flags target)
  if(ENABLE_COVERAGE)
      message("Setting build type to Debug. Coverage works better with Debug flags.")
      set(CMAKE_BUILD_TYPE Debug)
      target_compile_options(${target}
          PRIVATE
          --coverage)
      target_link_options(${target} PRIVATE --coverage)
  endif()
endfunction()

# We specify the the libraries as OBJECT libraries so that they are compiled but
# not linked into archives. This allows us to specify a specific order for the
# object files later. Also note that it is important that the file list below be
# preserved. Changing the order can cause booting to fail.
add_library(cpp_sources OBJECT
            src/main.cpp
            src/cxxabi.cpp
            src/global_constructor_test.cpp
            src/pure_virtual.cpp
            src/local_static_variable_test.cpp
            src/serial.cpp
            src/terminal.cpp)
add_clang_tidy(cpp_sources)
add_coverage_flags(cpp_sources)
target_include_directories(cpp_sources PUBLIC include/ third-party/libgcov-embedded/port/src/)
if(ENABLE_COVERAGE)
    target_compile_definitions(cpp_sources PRIVATE "ENABLE_LIBGCOV_PORT=1")
endif()
target_compile_options(cpp_sources
                       PRIVATE "-ffreestanding"
                               "-nostdlib"
                               "-fno-exceptions"
                               "-fno-rtti")

add_library(crti OBJECT src/crti.nasm)
add_library(crtn OBJECT src/crtn.nasm)
add_library(boot OBJECT src/boot.nasm)

add_library(asm_sources OBJECT src/isr.nasm src/descriptor.nasm)

add_library(tinyprintf OBJECT src/tinyprintf.c)
target_include_directories(tinyprintf PUBLIC include/)
set_target_properties(tinyprintf PROPERTIES LINKER_LANGUAGE C)
target_compile_options(tinyprintf PRIVATE "-ffreestanding" "-nostdlib")

# This is probably unnecessary. But for now, it's convenient to allow us to put
# the flags at the beginning rather than the end.
set(
  CMAKE_CXX_LINK_EXECUTABLE
  "<CMAKE_LINKER> -T ${CMAKE_SOURCE_DIR}/linker.ld --coverage -ffreestanding -nostdlib -fno-exceptions -fno-rtti <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> -o <TARGET> <OBJECTS> <LINK_LIBRARIES>"
  )

execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=crtbegin.o
                OUTPUT_VARIABLE CRTBEGIN)
execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=crtend.o
                OUTPUT_VARIABLE CRTEND)

# Remove trailing newline from output.
string(STRIP "${CRTBEGIN}" CRTBEGIN)
string(STRIP "${CRTEND}" CRTEND)

# Inform CMake that this is a prebuilt object.
set_source_files_properties(${CRTBEGIN}
                            PROPERTIES
                            EXTERNAL_OBJECT
                            true
                            GENERATED
                            true)

set_source_files_properties(${CRTEND}
                            PROPERTIES
                            EXTERNAL_OBJECT
                            true
                            GENERATED
                            true)

if(ENABLE_COVERAGE)
    add_subdirectory(third-party)
endif()
# As stated previously, the order of these should not change.
add_executable(kernel
               $<TARGET_OBJECTS:crti>
               ${CRTBEGIN}
               $<TARGET_OBJECTS:boot>
               $<TARGET_OBJECTS:cpp_sources>
               $<TARGET_OBJECTS:asm_sources>
               $<TARGET_OBJECTS:tinyprintf>
               $<$<BOOL:${ENABLE_COVERAGE}>:
                   $<TARGET_OBJECTS:libgcov_src>
                   $<TARGET_OBJECTS:libgcov_libgcc>
                   $<TARGET_OBJECTS:libgcov_gcc>>
               ${CRTEND}
               $<TARGET_OBJECTS:crtn>)
target_link_libraries(kernel PUBLIC gcc $<$<BOOL:${ENABLE_COVERAGE}>:gcov>)

# make iso expects the output binary to be in this directory with this name.
set_target_properties(kernel
                      PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                 "${PROJECT_SOURCE_DIR}"
                                 OUTPUT_NAME
                                 "myos.bin")
