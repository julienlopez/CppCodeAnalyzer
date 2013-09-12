#!/bin/bash

BUILD_DIR=build_tools
[ -d "$BUILD_DIR" ] && rm -rf $BUILD_DIR
mkdir $BUILD_DIR && cd $BUILD_DIR && cmake ../tools/lcovhtmlcleaner -G"Unix Makefiles" && make VERBOSE=1