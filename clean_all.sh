#!/bin/bash
cd util
make distclean
cd ../axiom
make distclean
cd ../xml_schema
make distclean
cd ../woden
make distclean
cd ..
sh clean.sh
rm -rf deploy

