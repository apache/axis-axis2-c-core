#!/bin/bash
set -e
sh autogen.sh

AXIS2C_HOME=`pwd`/deploy
export AXIS2C_HOME
echo "AXIS2C_HOME = ${AXIS2C_HOME}"

sh configure --prefix=`pwd`/deploy --enable-tests=yes --with-apache2=/usr/local/apache2/include --enable-tcp=yes --with-archive=/usr/include
make -j 10
make install

cd samples
sh autogen.sh
sh configure --prefix=`pwd`/../deploy --with-axis2=`pwd`/../deploy/include/axis2-1.7.0
make -j 10
make install
cd ..
