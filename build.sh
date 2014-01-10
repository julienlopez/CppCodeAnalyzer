#!/bin/bash

source build_directories.sh

BUILD_DIR=build
[ -d "$BUILD_DIR" ] || mkdir $BUILD_DIR
cd $BUILD_DIR && cmake ../src -G"Unix Makefiles" -Dogdf_include_directory:PATH=$OGDF_INCLUDE_PATH -Dogdf_lib_directory:PATH=$OGDF_DEBUG_LIB_PATH -Dcoin-osi_lib_directory:PATH=$COIN_OSI_LIB_PATH -Dcoin-clp_lib_directory:PATH=$COIN_OSI_LIB_PATH  && make
