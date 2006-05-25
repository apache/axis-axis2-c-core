#!/bin/bash
./autogen.sh
AXIS2C_HOME=`pwd`/deploy
AXIS2C=`pwd`
export AXIS2C_HOME AXIS2C

./configure --prefix=${AXIS2C_HOME} --enable-tests=yes --enable-trace=yes  --with-apache2=/usr/include/apache2 --with-apr=/usr/include/apr-0 --enable-diclient=no --enable-static=no --enable-openssl=no --with-axis2_util=${AXIS2C}/util/include --with-axiom=${AXIS2C}/axiom/include 2> error.log

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

cd samples

./autogen.sh

./configure --prefix=${AXIS2C_HOME} --with-axis2_util=${AXIS2C_HOME}/include --with-axiom=${AXIS2C_HOME}/include 2> error.log
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
								
cd ..
