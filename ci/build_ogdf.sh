#!/bin/bash
wget http://www.ogdf.net/lib/exe/fetch.php/tech:ogdf.v2012.07.zip
unzip tech\:ogdf.v2012.07.zip
cd OGDF
./makeMakefile.sh 
make
ls -la