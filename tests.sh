#!/bin/bash

source build_directories.sh

BUILD_DIR=build_tests
OBJ_DIR=CMakeFiles/tests.dir

function error_exit
{
	echo "$1" 1>&2
	exit 1
}

#[ -d "$BUILD_DIR" ] && rm -rf $BUILD_DIR
#mkdir $BUILD_DIR && cd $BUILD_DIR && cmake ../tests -G"Unix Makefiles" -Dogdf_include_directory:PATH=$OGDF_INCLUDE_PATH -Dogdf_lib_directory:PATH=$OGDF_DEBUG_LIB_PATH && make VERBOSE=1
cd $BUILD_DIR && cmake ../tests -G"Unix Makefiles" -Dogdf_include_directory:PATH=$OGDF_INCLUDE_PATH -Dogdf_lib_directory:PATH=$OGDF_DEBUG_LIB_PATH && make VERBOSE=1

[ $? -ne 0 ] && error_exit "Compile step failed."

lcov --zerocounters --directory $OBJ_DIR --no-external > lcov.log 2> lcov.err
lcov --capture --initial --directory $OBJ_DIR --no-external --output-file app >> lcov.log 2>> lcov.err

../bin/tests

echo "analyzing coverage data..."

lcov --no-checksum --directory $OBJ_DIR --directory ../src --no-external --capture --output-file app.info >> lcov.log 2>> lcov.err
genhtml -o html app.info >> lcov.log 2>> lcov.err

echo "done!"

