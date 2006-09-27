#!/bin/bash

./autogen.sh

if test -z ${AXIS2C_HOME}
then
    AXIS2C_HOME=`pwd`/../deploy
fi

export AXIS2C_HOME

./configure --prefix=${AXIS2C_HOME} --enable-tests=no --with-axis2_util=${AXIS2C_HOME}/include 
make
make install
