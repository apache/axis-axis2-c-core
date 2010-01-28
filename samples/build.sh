#!/bin/bash

./autogen.sh

./configure --prefix=${AXIS2C_HOME}
make 
make install

