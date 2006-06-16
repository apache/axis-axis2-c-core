#!/bin/bash

cd util
sh build.sh
cd ../axiom
sh build.sh
cd ../xml_schema
sh build.sh
cd ..
sh build.sh
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

