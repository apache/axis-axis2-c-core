#!/bin/bash
./autogen.sh
./configure --prefix=${AXIS2C_HOME} --enable-static=no --enable-tests=yes
make
make install
