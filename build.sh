#!/bin/bash
set -e
sh autogen.sh
AXIS2C_HOME=${AXIS2C_HOME:=`pwd`/deploy}

export AXIS2C_HOME

echo "AXIS2C_HOME = ${AXIS2C_HOME}"

sh configure --with-apache2=/home/dummy/tmp/httpd-2.2.3/deploy/include --prefix=${AXIS2C_HOME}
make 
make install

cd samples
sh autogen.sh
sh configure --prefix=${AXIS2C_HOME} 
make 
make install
								
cd ..
