#!/bin/bash
./autogen.sh
./configure --prefix=${AXIS2C_HOME}/rampart --enable-static=no --with-axis2_util=${AXIS2C_HOME}/include --with-axiom=${AXIS2C_HOME}/include
make
make install
