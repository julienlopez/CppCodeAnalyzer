#!/bin/bash

BUILD_DIR=build_tests
OBJ_DIR=CMakeFiles/tests.dir

[ -d "$BUILD_DIR" ] && rm -rf $BUILD_DIR
mkdir $BUILD_DIR && cd $BUILD_DIR && cmake ../tests -G"Unix Makefiles" && make VERBOSE=1
lcov --zerocounters --directory $OBJ_DIR
lcov --capture --initial --directory $OBJ_DIR --output-file app
../bin/tests
lcov --no-checksum --directory $OBJ_DIR --capture --output-file app.info
genhtml -o html app.info

cp html/index.html html/index.html.bak
../bin/lcovHTMLcleaner html/index.html
