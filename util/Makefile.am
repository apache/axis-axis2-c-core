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
# AXIS2C-1338: Removed datadir=$(prefix) and data_DATA which installed
# documentation files to wrong location. These files are in EXTRA_DIST
# for distribution but don't need separate installation as a subproject.
tmpincludedir=$(prefix)/include/axis2-${PACKAGE_VERSION}/
includedir=$(prefix)/include/axis2-${PACKAGE_VERSION}/
SUBDIRS = src $(GTEST) $(TESTDIR)
include_HEADERS=$(top_srcdir)/include/*.h

platformincludedir=$(includedir)/platforms/
platforminclude_HEADERS=$(top_srcdir)/include/platforms/*.h

unixincludedir=$(platformincludedir)/unix/
unixinclude_HEADERS= $(top_srcdir)/include/platforms/unix/*.h

os400includedir=$(platformincludedir)/os400/
os400include_HEADERS= $(top_srcdir)/include/platforms/os400/*.h

windowsincludedir=$(platformincludedir)/windows/
windowsinclude_HEADERS= $(top_srcdir)/include/platforms/windows/*.h

tmpinclude_DATA=config.h
EXTRA_DIST = build.sh autogen.sh CREDITS LICENSE gtest
dist-hook:
	rm -rf `find $(distdir)/ -type d -name .git`
	rm -rf `find $(distdir)/ -type d -name .deps`
