#!/bin/bash
./autogen.sh
AXIS2C_HOME=${AXIS2C_HOME:=`pwd`/deploy}

export AXIS2C_HOME

echo "AXIS2C_HOME = ${AXIS2C_HOME}"

./configure --prefix=${AXIS2C_HOME}
make 
make install

cd samples
./autogen.sh
./configure --prefix=${AXIS2C_HOME} --with-axis2_util=${AXIS2C_HOME}/include --with-axiom=${AXIS2C_HOME}/include
make 
make install
								
cd ..
