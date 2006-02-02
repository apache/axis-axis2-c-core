#!/bin/bash
./autogen.sh
AXIS2C_HOME=`pwd`/deploy
./configure --prefix=${AXIS2C_HOME} --enable-tests=yes --enable-samples=yes
make
make install
