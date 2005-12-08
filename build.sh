#!/bin/bash
./autogen.sh
AXIS2C_HOME=`pwd`
./configure --prefix=${AXIS2C_HOME}/install
make
make install
