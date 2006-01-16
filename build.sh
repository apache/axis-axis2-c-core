#!/bin/bash
./autogen.sh
AXIS2C_HOME=`pwd`/deploy
./configure --prefix=${AXIS2C_HOME}
make
make install
cp deploy/lib/libaxis2_libxml2.so deploy/lib/libaxis2_parser.so
