#!/bin/bash
./autogen.sh
AXIS2C_HOME=`pwd`/deploy
./configure --prefix=${AXIS2C_HOME} --enable-tests=yes --enable-samples=yes
make
make install
cp -f ${AXIS2C_HOME}/lib/libecho.so ${AXIS2C_HOME}/services/echo
cp -f ${AXIS2C_HOME}/lib/libmath.so ${AXIS2C_HOME}/services/math
