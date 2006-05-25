#!/bin/bash

./autogen.sh

./configure --prefix=${AXIS2C_HOME} --with-axis2_util=${AXIS2C_HOME}/include --with-axiom=${AXIS2C_HOME}/include
if [ -s error.log ]
then
    echo "Error, see error.log for details"
	exit 2
fi

make  2>error.log
	if grep -w 'Error' `find -name "error.log"`; then
	echo "Error, see error.log for details"
	exit 2
fi

make install 2>error.log
	if grep -w 'Error' `find -name "error.log"`; then
	echo "Error, see error.log for details"
	exit 2
fi

