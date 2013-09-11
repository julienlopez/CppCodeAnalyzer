cmake_minimum_required(VERSION 2.8)

project(CppCodeAnalyzer)

set(EXECUTABLE_OUTPUT_PATH ../bin)

ADD_DEFINITIONS(-std=c++11)

include_directories(.)

file(
        GLOB_RECURSE
        source_files
        ./*
)

add_executable(CppCodeAnalyzer ${source_files})

target_link_libraries(
        CppCodeAnalyzer

        boost_program_options 
        boost_system 
        boost_filesystem
)
