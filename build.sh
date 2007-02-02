#!/bin/bash
set -e
./autogen.sh
AXIS2C_HOME=${AXIS2C_HOME:=`pwd`/deploy}

export AXIS2C_HOME

echo "AXIS2C_HOME = ${AXIS2C_HOME}"

./configure --prefix=${AXIS2C_HOME} 
make 
make install

cd samples
./autogen.sh
./configure --prefix=${AXIS2C_HOME} 
make 
make install
								
cd ..
