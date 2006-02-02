#!/bin/bash
./autogen.sh
AXIS2C_HOME=`pwd`/deploy
./configure --prefix=${AXIS2C_HOME} --enable-tests=yes --enable-samples=yes
make
make install
cp -f samples/server/echo/services.xml ${AXIS2C_HOME}/services/echo
cp -f samples/server/math/services.xml ${AXIS2C_HOME}/services/math
cp -f samples/server/axis2.xml ${AXIS2C_HOME}
