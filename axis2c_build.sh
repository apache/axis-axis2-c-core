#!/bin/bash

if [ -z "$1" ] ; then
    AXIS2C_DEPLOY="/usr/local/axis2"
else
    AXIS2C_DEPLOY=$1
fi

export LD_LIBRARY_PATH=${AXIS2C_DEPLOY}/lib
cd util

./configure --prefix=${AXIS2C_DEPLOY}
make 
make install

cd ../axiom
./configure --prefix=${AXIS2C_DEPLOY} --with-axis2_util=${AXIS2C_DEPLOY}/include
make 
make install

cd ../xml_schema
./configure --prefix=${AXIS2C_DEPLOY} --with-axis2_util=${AXIS2C_DEPLOY}/include --with-axiom=${AXIS2C_DEPLOY}/include
make 
make install

cd ../woden
./configure --prefix=${AXIS2C_DEPLOY} --with-axis2_util=${AXIS2C_DEPLOY}/include --with-axiom=${AXIS2C_DEPLOY}/include
make 
make install

cd ..
./configure --prefix=${AXIS2C_DEPLOY} --with-axis2_util=${AXIS2C_DEPLOY}/include --with-axiom=${AXIS2C_DEPLOY}/include
make 
make install

cd rampart
./configure --prefix=${AXIS2C_DEPLOY} --with-axis2_util=${AXIS2C_DEPLOY}/include --with-axiom=${AXIS2C_DEPLOY}/include
make 
make install

cd ../samples
./configure --prefix=${AXIS2C_DEPLOY} --with-axis2_util=${AXIS2C_DEPLOY}/include --with-axiom=${AXIS2C_DEPLOY}/include
make 
make install


