#!/bin/bash
cd util
sh build.sh
cd ..
cd axiom
sh build.sh
cd ..
./autogen.sh
AXIS2C_HOME=`pwd`/deploy
AXIS2C=`pwd`
export AXIS2C_HOME AXIS2C
./configure --prefix=${AXIS2C_HOME} --enable-tests=yes --enable-trace=yes  --with-apache2=/usr/include/apache2 --with-apr=/usr/include/apr-0 --enable-diclient=no --enable-static=no --enable-openssl=no --with-axis2_util=${AXIS2C}/util/include --with-axiom=${AXIS2C}/axiom/include
make
make install
cd samples
sh build.sh
cd ..
