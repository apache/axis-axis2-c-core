#!/bin/bash
./autogen.sh
./configure --prefix=${AXIS2C_HOME}/rampart --enable-static=no 
make
make install
