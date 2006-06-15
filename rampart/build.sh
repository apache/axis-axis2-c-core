#!/bin/bash
./autogen.sh
./configure --prefix=${AXIS2C_HOME}/rampart --enable-static=no --with-axis2_util=${AXIS2C_HOME}/include --with-axis2_axiom=${AXIS2C_HOME}/include --with-axis2_xml_schema=${AXIS2C_HOME}/include
make
make install
