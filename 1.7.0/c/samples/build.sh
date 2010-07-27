#!/bin/bash

./autogen.sh

./configure --prefix=${AXIS2C_HOME}
make -j 10
make install

