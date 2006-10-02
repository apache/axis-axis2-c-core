#!/bin/bash
sh clean.sh

./autogen.sh
AXIS2C_HOME=`pwd`/deploy
export AXIS2C_HOME 

./configure --prefix=${AXIS2C_HOME} --enable-tests=no --enable-trace=yes  --enable-diclient=no --enable-static=no --enable-openssl=no 

make
make install

cd samples

./autogen.sh

./configure --prefix=${AXIS2C_HOME} --with-axis2_util=${AXIS2C_HOME}/include --with-axiom=${AXIS2C_HOME}/include
make
make install

make dist
tar xf axis2c-sample-src-0.94.tar.gz
mv axis2c-sample-src-0.94 samples
rm -rf ${AXIS2C_HOME}/samples
mv samples ${AXIS2C_HOME}

cd ..
rm -rf xdocs/api/html
maven site
cd xdocs/api 
doxygen doxygenconf
cd ../..
cp -r xdocs/api/html target/docs/api/
cp -r target/docs ${AXIS2C_HOME}

cd ${AXIS2C_HOME}

# rm -rf config.sub missing config.guess depcomp ltmain.sh
for i in `find . -name "*.la"`
do
   rm $i
done

for i in `find . -name "*.a"`
do
   rm $i
done

