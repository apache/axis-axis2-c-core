#!/bin/bash
set -e
sh autogen.sh
AXIS2C_HOME=${AXIS2C_HOME:=`pwd`/deploy}

export AXIS2C_HOME

echo "AXIS2C_HOME = ${AXIS2C_HOME}"

sh configure --prefix=${AXIS2C_HOME} --enable-tests=yes --with-apache2=/usr/local/apache2/include --with-apr=/usr/local/apache2/include --enable-tcp
make 
make install

cd samples
sh autogen.sh
sh configure --prefix=${AXIS2C_HOME} --with-axis2=${AXIS2C_HOME}/include/axis2-1.1
make 
make install
								
cd ..
