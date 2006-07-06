#!/bin/bash
./autogen.sh
AXIS2C=`pwd`/../../projects/c

./configure --prefix=${AXIS2C_HOME} --enable-static=no --with-axis2_util=${AXIS2C_HOME}/include --with-axis2_axiom=${AXIS2C_HOME}/include --with-axis2_xml_schema=${AXIS2C}/xml_schema/include
make
make install
