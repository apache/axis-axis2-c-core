#!/usr/bin/bash
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
#==================================
# There is a Jira issue AXIS2C-1437 as following
#On AIX, the dynamic are correctly built, but they are not installed by "make install". I only have .a and .la in $deploy_dir/lib. No .so. It looks like that the dynamic libraries are build with a ".so.0" extension instead of just ".so" 
#==================================
#A workaround script is provided under the same jira to manually copy .so.0 to .so. This is it

[[ -z "$deploy_dir" ]] && exit 2

cd ${1:-axis2c-src-1.6.0}

cp $(find . -name '*.so.0') $deploy_dir/lib
cd $deploy_dir/lib
for f in *.so.0
do
	x=$(basename $f .0)
	[[ -h $x ]] || ln -s -f $f $x
done
mv libaxis2_mod_addr.* ../modules/addressing
mv libaxis2_mod_log.* ../modules/logging
