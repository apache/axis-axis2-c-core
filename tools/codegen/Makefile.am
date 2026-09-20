# Makefile.am for tools/codegen directory
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

# Build native codegen if enabled
# The native code generator (wsdl2c-native and its test runner) is a host
# tool: it links libxml2, which an Android dependency prefix does not carry,
# and nothing on a phone runs a WSDL code generator. Building it on a
# cross-build failed the whole tree at the very end, after every library had
# already been produced. Skip the directory when configured for Android.
if AXIS2_NATIVE_CODEGEN_ENABLED
if !BUILD_FOR_ANDROID
SUBDIRS = native
endif
endif

# Extra files to distribute
EXTRA_DIST = \
    README \
    javatool/