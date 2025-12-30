#!/bin/bash
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
# 
#   http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
set -e

# Clean stale objects from previous builds (especially HTTP/2 builds)
# This ensures WITH_NGHTTP2 code isn't mixed with non-HTTP/2 builds
if [ -f Makefile ]; then
    echo "Cleaning stale objects from previous build..."
    make distclean || make clean || true
fi

# Also clean any .libs directories that distclean might miss
find . -type d -name ".libs" -exec rm -rf {} + 2>/dev/null || true

sh autogen.sh
AXIS2C_HOME=${AXIS2C_HOME:=`pwd`/deploy}

export AXIS2C_HOME

echo "AXIS2C_HOME = ${AXIS2C_HOME}"

sh configure --prefix=${AXIS2C_HOME} \
    --enable-tests --with-gtest=/usr/src/googletest/googletest \
    --enable-coverage \
    --enable-asan \
    --enable-json=yes
make -j 10 
make install

cd samples
sh build.sh

cd ..
