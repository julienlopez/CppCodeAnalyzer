cmake_minimum_required(VERSION 2.8)

project(lcovHTMLcleaner)

set(EXECUTABLE_OUTPUT_PATH ../bin)

ADD_DEFINITIONS(-std=c++11)

include_directories(../../src)

file(
        GLOB_RECURSE
        source_files
        ./*
)
list(APPEND source_files "../src/utils/stringhelper.hpp")
list(APPEND source_files "../src/utils/stringhelper.cpp")

add_executable(lcovHTMLcleaner ${source_files})

target_link_libraries(
        lcovHTMLcleaner

#        boost_program_options 
        boost_system 
        boost_filesystem
)
