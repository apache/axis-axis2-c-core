#!/bin/bash

./autogen.sh

./configure --prefix=${AXIS2C_HOME} --with-axis2_util=${AXIS2C_HOME}/include/axis2-1.1 --with-axiom=${AXIS2C_HOME}/include/axis2-1.1
make 
make install

