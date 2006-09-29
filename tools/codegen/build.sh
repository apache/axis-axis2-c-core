#!/bin/bash

./autogen.sh

if test -z ${AXIS2C_HOME}
then
    AXIS2C_HOME=`pwd`/../deploy
fi

AXIS2C=`pwd`/..
export AXIS2C_HOME AXIS2C

./configure --prefix=${AXIS2C_HOME} --enable-tests=no

make
make install
