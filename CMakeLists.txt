cmake_minimum_required(VERSION 3.14.5)

project(Cpp-i386-kernel CXX)

set(STANDARDESE_TOOL ${CMAKE_SOURCE_DIR}/standardese)
include(${CMAKE_SOURCE_DIR}/standardese-config.cmake)

set(include_dirs
    include/
    include/kstd)

set(headers
    include/multiboot2.hpp
    include/gdt.hpp
    include/isr.hpp
    include/inline_asm.hpp
    include/idt.hpp
    include/Terminal.hpp
    include/Terminal_iterator.hpp
    include/kstd/algorithm.hpp
    include/kstd/basic_string_view.hpp
    include/kstd/string.hpp
    include/kstd/array.hpp
    include/kstd/iterator.hpp
    include/kstd/utilities.hpp
    include/kstd/char_traits.hpp)

standardese_generate(standardese INCLUDE_DIRECTORY ${include_dirs}
                     INPUT ${headers})
