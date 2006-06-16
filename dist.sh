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

tar xf axis2c-src-0.92.tar.gz 
if [ "$?" -ne 0 ]
then
exit 1
fi

rm -rf util

mv axis2c-src-0.92 util
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

tar xf axis2c-src-0.92.tar.gz 
if [ "$?" -ne 0 ]
then
exit 1
fi

rm -rf axiom

mv axis2c-src-0.92 axiom
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

tar xf axis2c-src-0.92.tar.gz 
if [ "$?" -ne 0 ]
then
exit 1
fi

rm -rf xml_schema

mv axis2c-src-0.92 xml_schema
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

tar xf axis2c-src-0.92.tar.gz 
if [ "$?" -ne 0 ]
then
exit 1
fi

rm -rf woden

mv axis2c-src-0.92 woden
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

cd rampart
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

tar xf axis2c-src-0.92.tar.gz 
if [ "$?" -ne 0 ]
then
exit 1
fi

rm -rf rampart

mv axis2c-src-0.92 rampart
if [ "$?" -ne 0 ]
then
exit 1
fi

cd ../release
if [ "$?" -ne 0 ]
then
exit 1
fi

mv ../axis2c-src-0.92 .
if [ "$?" -ne 0 ]
then
exit 1
fi

mv ../axiom/axiom axis2c-src-0.92
if [ "$?" -ne 0 ]
then
exit 1
fi

mv ../xml_schema/xml_schema axis2c-src-0.92
if [ "$?" -ne 0 ]
then
exit 1
fi

mv ../woden/woden axis2c-src-0.92
if [ "$?" -ne 0 ]
then
exit 1
fi

mv ../rampart/rampart axis2c-src-0.92
if [ "$?" -ne 0 ]
then
exit 1
fi

mv ../util/util axis2c-src-0.92
if [ "$?" -ne 0 ]
then
exit 1
fi

cd ..
if [ "$?" -ne 0 ]
then
exit 1
fi

rm -rf axis2c-src-0.92
if [ "$?" -ne 0 ]
then
exit 1
fi

mv release/axis2c-src-0.92 .
if [ "$?" -ne 0 ]
then
exit 1
fi

