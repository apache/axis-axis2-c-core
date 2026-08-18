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

# The samples install into the Axis2/C installation they are built against, and
# find their headers under ${AXIS2C_HOME}/include. Unset, --prefix= below is
# empty and configure quietly accepts it: the build then fails with
# "axis2_svc_skeleton.h: No such file or directory" for every service, which
# reads as a missing dependency rather than as a missing variable, and the
# services never deploy -- so the transport tests that need the echo service
# fail afterwards for what looks like an unrelated reason. Refuse instead.
if [ -z "${AXIS2C_HOME}" ]; then
    echo "build.sh: AXIS2C_HOME is not set." >&2
    echo "  Set it to the installation these samples should build against and" >&2
    echo "  deploy into, e.g." >&2
    echo "      export AXIS2C_HOME=/path/to/axis2c-install" >&2
    exit 1
fi

if [ ! -d "${AXIS2C_HOME}/include" ]; then
    echo "build.sh: no include directory under AXIS2C_HOME (${AXIS2C_HOME})." >&2
    echo "  Build and 'make install' Axis2/C itself before the samples." >&2
    exit 1
fi

if ! sh autogen.sh; then
    echo "build.sh: autogen.sh failed; not continuing with a stale configure." >&2
    exit 1
fi

./configure --prefix=${AXIS2C_HOME}
make clean || true
make -j 4
make install

