#!/bin/bash
./autogen.sh
AXIS2C=`pwd`/..

./configure --enable-static=no --with-axis2_util=${AXIS2C_HOME}/include --with-axis2_axiom=${AXIS2C_HOME}/include --with-axis2_xml_schema=${AXIS2C}/xml_schema/include
make
