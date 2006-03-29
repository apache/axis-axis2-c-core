#!/bin/bash
./autogen.sh
./configure --prefix=${AXIS2C_HOME} --enable-diclient=yes
make
make install
