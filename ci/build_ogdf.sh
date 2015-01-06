#!/bin/bash
wget http://www.ogdf.net/lib/exe/fetch.php/tech:ogdf.v2012.07.zip
unzip tech\:ogdf.v2012.07.zip
cd OGDF
./makeMakefile.sh 
make
sudo ln -s /home/travis/build/julienlopez/CppCodeAnalyzer/OGDF/ogdf /usr/include
sudo ln -s /home/travis/build/julienlopez/CppCodeAnalyzer/OGDF/_release/libOGDF.a /usr/lib64
ls -la
ls -l /usr/include/ogdf
ls -l /usr/lib64/libOGDF*