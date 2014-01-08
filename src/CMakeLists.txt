cmake_minimum_required(VERSION 2.8)

project(CppCodeAnalyzer)

set(EXECUTABLE_OUTPUT_PATH ../bin)

ADD_DEFINITIONS(-std=c++11 -g -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Wmissing-declarations -Wredundant-decls -Winline -Wno-long-long -Wuninitialized -Wconversion)
SET(CMAKE_EXE_LINKER_FLAGS "-g")

include_directories(.)

include_directories(PATH_TO_ODGF)

link_directories(PATH_TO_ODGF/_debug)
link_directories(PATH_TO_COIN-OSI/lib)
link_directories(PATH_TO_COIN-CLP/lib)

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
        OGDF
        CoinUtils
        Osi
        OsiClp
        Clp
        z
        lapack
)
