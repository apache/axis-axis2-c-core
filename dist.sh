#!/bin/bash

rm -rf release
if [ "$?" -ne 0 ]
then
exit 1
fi

mkdir release
if [ "$?" -ne 0 ]
then
exit 1
fi


cd util
if [ "$?" -ne 0 ]
then
exit 1
fi

sh build.sh
if [ "$?" -ne 0 ]
then
exit 1
fi

make dist
if [ "$?" -ne 0 ]
then
exit 1
fi

tar xf axis2c-src-0.93.tar.gz 
if [ "$?" -ne 0 ]
then
exit 1
fi

rm -rf util

mv axis2c-src-0.93 util
if [ "$?" -ne 0 ]
then
exit 1
fi

cd ../axiom
if [ "$?" -ne 0 ]
then
exit 1
fi

sh build.sh
if [ "$?" -ne 0 ]
then
exit 1
fi

make dist
if [ "$?" -ne 0 ]
then
exit 1
fi

tar xf axis2c-src-0.93.tar.gz 
if [ "$?" -ne 0 ]
then
exit 1
fi

rm -rf axiom

mv axis2c-src-0.93 axiom
if [ "$?" -ne 0 ]
then
exit 1
fi

cd ../xml_schema
if [ "$?" -ne 0 ]
then
exit 1
fi

sh build.sh
if [ "$?" -ne 0 ]
then
exit 1
fi

make dist
if [ "$?" -ne 0 ]
then
exit 1
fi

tar xf axis2c-src-0.93.tar.gz 
if [ "$?" -ne 0 ]
then
exit 1
fi

rm -rf xml_schema

mv axis2c-src-0.93 xml_schema
if [ "$?" -ne 0 ]
then
exit 1
fi

cd ../woden
if [ "$?" -ne 0 ]
then
exit 1
fi

sh build.sh
if [ "$?" -ne 0 ]
then
exit 1
fi

make dist
if [ "$?" -ne 0 ]
then
exit 1
fi

tar xf axis2c-src-0.93.tar.gz 
if [ "$?" -ne 0 ]
then
exit 1
fi

rm -rf woden

mv axis2c-src-0.93 woden
if [ "$?" -ne 0 ]
then
exit 1
fi

cd ..
if [ "$?" -ne 0 ]
then
exit 1
fi

sh build.sh
if [ "$?" -ne 0 ]
then
exit 1
fi

cd samples
if [ "$?" -ne 0 ]
then
exit 1
fi

make clean
if [ "$?" -ne 0 ]
then
exit 1
fi

sh build.sh
if [ "$?" -ne 0 ]
then
exit 1
fi

make dist
if [ "$?" -ne 0 ]
then
exit 1
fi

tar xf axis2c-src-0.93.tar.gz 
if [ "$?" -ne 0 ]
then
exit 1
fi

rm -rf samples

mv axis2c-src-0.93 samples
if [ "$?" -ne 0 ]
then
exit 1
fi

cd ../rampart
if [ "$?" -ne 0 ]
then
exit 1
fi

sh build.sh
if [ "$?" -ne 0 ]
then
exit 1
fi

make dist
if [ "$?" -ne 0 ]
then
exit 1
fi

tar xf axis2c-src-0.93.tar.gz 
if [ "$?" -ne 0 ]
then
exit 1
fi

rm -rf rampart

mv axis2c-src-0.93 rampart
if [ "$?" -ne 0 ]
then
exit 1
fi

cd ../release
if [ "$?" -ne 0 ]
then
exit 1
fi

mv ../axis2c-src-0.93 .
if [ "$?" -ne 0 ]
then
exit 1
fi

mv ../axiom/axiom axis2c-src-0.93
if [ "$?" -ne 0 ]
then
exit 1
fi

mv ../xml_schema/xml_schema axis2c-src-0.93
if [ "$?" -ne 0 ]
then
exit 1
fi

mv ../woden/woden axis2c-src-0.93
if [ "$?" -ne 0 ]
then
exit 1
fi

mv ../rampart/rampart axis2c-src-0.93
if [ "$?" -ne 0 ]
then
exit 1
fi

mv ../util/util axis2c-src-0.93
if [ "$?" -ne 0 ]
then
exit 1
fi

rm -rf axis2c-src-0.93/samples
mv ../samples/samples axis2c-src-0.93
if [ "$?" -ne 0 ]
then
exit 1
fi

cd ..
if [ "$?" -ne 0 ]
then
exit 1
fi

rm -rf axis2c-src-0.93
if [ "$?" -ne 0 ]
then
exit 1
fi

mv release/axis2c-src-0.93 .
if [ "$?" -ne 0 ]
then
exit 1
fi

rm -rf xdocs/api/html
maven site
cd xdocs/api
doxygen doxygenconf
cd ../..
cp -r xdocs/api/html target/docs/api/
cp -r target/docs axis2c-src-0.93

for i in `find . -name "*.la"`
do
	rm $i
done

for i in `find . -name "*.a"`
do
	rm $i
done

