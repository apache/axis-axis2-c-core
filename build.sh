#!/bin/bash
AXIS2C_HOME=`pwd`
./configure --prefix=${AXIS2C_HOME}/install --with-pull-parser=libxml2
make
make install
