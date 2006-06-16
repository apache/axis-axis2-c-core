rm -rf release
mkdir release

cd util
sh build.sh
make dist
tar xf axis2c-src-0.92.tar.gz 
mv axis2c-src-0.92 util

cd ../axiom
sh build.sh
make dist
tar xf axis2c-src-0.92.tar.gz 
mv axis2c-src-0.92 axiom

cd ../xml_schema
sh build.sh
make dist
tar xf axis2c-src-0.92.tar.gz 
mv axis2c-src-0.92 xml_schema

cd ../woden
sh build.sh
make dist
tar xf axis2c-src-0.92.tar.gz 
mv axis2c-src-0.92 woden

cd ..
sh build.sh

cd rampart
sh build.sh
make dist
tar xf axis2c-src-0.92.tar.gz 
mv axis2c-src-0.92 rampart

cd ../release
mv ../axis2c-src-0.92.tar.gz .
tar xf axis2c-src-0.92.tar.gz
mv ../axiom/axiom axis2c-src-0.92 
mv ../xml_schema/xml_schema axis2c-src-0.92 
mv ../woden/woden axis2c-src-0.92 
mv ../rampart/rampart axis2c-src-0.92 
mv ../util/util axis2c-src-0.92
