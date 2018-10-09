#!/bin/bash
cd /usr/src/myapp
rm -rf build
mkdir build
cd build
cmake ..
make -j8
/usr/src/myapp/build/solucoes
