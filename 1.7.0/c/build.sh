#!/bin/bash
set -e
sh autogen.sh
AXIS2C_HOME=${AXIS2C_HOME:=`pwd`/deploy}

export AXIS2C_HOME

echo "AXIS2C_HOME = ${AXIS2C_HOME}"

sh configure --prefix=${AXIS2C_HOME} --enable-tests=yes
make -j 10 
make install

cd samples
sh build.sh

cd ..
