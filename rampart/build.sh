#!/bin/bash
./autogen.sh
RAMPART_HOME=`pwd`/target
./configure --prefix=${RAMPART_HOME} --enable-static=no --with-axis2_util=${AXIS2C_HOME}/include --with-axis2_axiom=${AXIS2C_HOME}/include --with-axis2_xml_schema=${AXIS2C_HOME}/include
make
make install
