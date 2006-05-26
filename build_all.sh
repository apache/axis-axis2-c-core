#!/bin/bash
cd util
sh build.sh  
cd ..
cd axiom
sh build.sh 
cd ..
sh build.sh 
cd samples
sh build.sh 
cd ..
