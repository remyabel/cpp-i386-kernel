cmake_minimum_required(VERSION 3.12.1)

project(Cpp-i386-kernel-tests CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(cpp-i386-kernel-tests
    ${CMAKE_CURRENT_SOURCE_DIR}/test-algorithm.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test-basicstringview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test-iterator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test-strlen.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test-array.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test-chartraits.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test-main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test-terminal.cpp)
target_include_directories(cpp-i386-kernel-tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/kstd/
    ${CONAN_INCLUDE_DIRS})
set_target_properties(cpp-i386-kernel-tests PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF)

target_link_libraries(cpp-i386-kernel-tests PRIVATE ${CONAN_LIBS})
