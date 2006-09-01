#!/bin/bash

./autogen.sh

if test -z ${AXIS2C_HOME}
then
    AXIS2C_HOME=`pwd`/../deploy
fi

AXIS2C=`pwd`/..
export AXIS2C_HOME AXIS2C

./configure --prefix=${AXIS2C_HOME} --enable-tests=yes --with-axis2_util=${AXIS2C_HOME}/include --with-axiom=${AXIS2C_HOME}/include --with-axis2_xml_schema=${AXIS2C_HOME}/include --with-axis2_woden=${AXIS2C_HOME}/include

make
make install
