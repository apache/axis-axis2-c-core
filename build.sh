#!/bin/bash
./autogen.sh
AXIS2C_HOME=`pwd`/deploy
./configure --prefix=${AXIS2C_HOME} --enable-tests=yes --enable-samples=yes --enable-trace=yes  --with-apache2=/usr/local/apache2
make
make install
