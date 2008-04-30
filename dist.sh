#!/bin/bash

rm -rf xdocs/api/html
maven site
cd xdocs/api
doxygen doxygenconf
cd ../..
cp -r xdocs/api/html target/docs/api/
cp xdocs/docs/mod_log/module.xml target/docs/docs/mod_log
rm target/docs/README
rm target/docs/dependencies.html
rm target/docs/downloads.html
rm target/xdocs/mail-lists.html
cp -r target/docs axis2c-src-1.4.0

cd samples   
make dist   
tar -xf axis2c-src-1.4.0.tar.gz     
mv  axis2c-src-1.4.0 ../axis2c-src-1.4.0    
cd ../axis2c-src-1.4.0      
mv axis2c-src-1.4.0 samples

for i in `find . -name "*.la"`
do
	rm $i
done

for i in `find . -name "*.a"`
do
	rm $i
done

for i in `find . -type d -name ".svn"`
do
        rm -rf $i
done
