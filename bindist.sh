#!/bin/bash

cd util
sh build.sh
cd ../axiom
sh build.sh
cd ../xml_schema
sh build.sh
cd ..

./autogen.sh

AXIS2C_HOME=`pwd`/deploy
AXIS2C=`pwd`
export AXIS2C_HOME AXIS2C

./configure --prefix=${AXIS2C_HOME} --enable-tests=yes --enable-trace=yes  --with-apache2=/usr/include/apache2 --with-apr=/usr/include/apr-0 --enable-diclient=no --enable-static=no --enable-openssl=no --with-axis2_util=${AXIS2C}/util/include --with-axiom=${AXIS2C}/axiom/include
make
make install

cd rampart
sh build.sh
cd ../woden
sh build.sh
cd ..
rm -rf xdocs/api/html
maven site
cd xdocs/api 
doxygen doxygenconf
cd ../..
cp -r xdocs/api/html target/docs/api/
cp -r target/docs ${AXIS2C_HOME}

cd ${AXIS2C_HOME}
for i in `find . -name "*.la"`
do
   rm $i
done

for i in `find . -name "*.a"`
do
   rm $i
done

