#!/bin/bash
./autogen.sh
./configure --enable-static=no --with-axis2_util=${AXIS2C_HOME}/include --with-axis2_axiom=${AXIS2C_HOME}/include --with-axis2_xml_schema=/home/damitha/projects/c/xml_schema/include
make
