#!/bin/bash

./autogen.sh
./configure --prefix=${AXIS2C_HOME} --enable-static=no --with-axis2_util=${AXIS2C_HOME}/include --enable-tests=yes
make 
make install

