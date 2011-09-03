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
sh autogen.sh

AXIS2C_HOME=`pwd`/deploy
export AXIS2C_HOME
echo "AXIS2C_HOME = ${AXIS2C_HOME}"

sh configure --prefix=${AXIS2C_HOME} --enable-tests=yes --with-apache2=/usr/include/apache2 --enable-tcp=yes --with-archive=/usr/include --with-apr=/usr/include/apr-1.0
make -j 10
make install

cd samples
sh autogen.sh
sh configure --prefix=${AXIS2C_HOME} --with-axis2=${AXIS2C_HOME}/include/axis2-1.7.0
make -j 10
make install
cd ..
