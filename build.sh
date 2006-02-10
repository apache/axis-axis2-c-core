#!/bin/bash
./autogen.sh
AXIS2C_HOME=`pwd`/deploy
./configure --prefix=${AXIS2C_HOME} --enable-tests=yes --enable-samples=yes --enable-trace=yes
make
make install
