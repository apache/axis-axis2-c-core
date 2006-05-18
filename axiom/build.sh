#!/bin/bash
./autogen.sh
./configure --enable-static=no --with-axis2_util=/home/damitha/projects/c/util/include
make
