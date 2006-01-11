#!/bin/bash
./autogen.sh
AXIS2C_HOME=`pwd`
./configure --prefix=${AXIS2C_HOME}/install
make
make install
cp install/lib/libaxis2_libxml2.so install/lib/libaxis2_parser.so
