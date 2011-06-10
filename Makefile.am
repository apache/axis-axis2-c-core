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
datadir=$(prefix)
logsdir=$(prefix)/logs
docsdir=$(prefix)
includedir=$(prefix)/include/axis2-${PACKAGE_VERSION}
wsdl2cdir=$(prefix)/bin/tools/wsdl2c


pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = axis2c.pc

SUBDIRS = util $(GUTHTHILA_DIR) axiom neethi src $(TESTDIR) include ides tools/tcpmon tools/md5
include_HEADERS=$(top_builddir)/include/*.h
data_DATA= samples/server/axis2.xml README \
	INSTALL CREDITS COPYING NEWS NOTICE AUTHORS
logs_DATA=
docs_DATA= 
wsdl2c_DATA=tools/codegen/javatool/WSDL2C.sh tools/codegen/javatool/README
EXTRA_DIST = CREDITS LICENSE build axis2c_build.sh guththila tools AUTHORS NOTICE tools/codegen/javatool/README  tools/codegen/javatool/WSDL2C.bat  tools/codegen/javatool/WSDL2C.sh docs docs/README

dist-hook:
	rm -rf `find $(distdir)/ -type d -name .svn`
	rm -rf `find $(distdir)/ -type f -name *.la`
	rm -rf `find $(distdir)/ -type f -name *.o`
	rm -rf `find $(distdir)/ -type f -name *.lo`
	rm -rf `find $(distdir)/ -type f -name *.loT`
	rm -rf `find $(distdir)/ -type d -name .deps`
	rm -rf `find $(distdir)/ -type d -name .libs`
	rm -rf `find $(distdir)/ -type f -name Makefile`
	rm -rf `find $(distdir)/ -type f -name axis2c-sample-src-${PACKAGE_VERSION}.tar.gz`
	rm -rf `find $(distdir)/ -type d -name autom4te.cache`
	rm -rf $(distdir)/tools/tcpmon/src/tcpmon
	rm -rf $(distdir)/tools/md5/src/md5
	find $(distdir) -name "makefile" | xargs sed -i "s/\/WX//g"
	sh dist.sh

bindist: $(bin_PROGRAMS)
	rm -rf axis2c-bin-${PACKAGE_VERSION}-linux
	sh bindist.sh
	mkdir axis2c-bin-${PACKAGE_VERSION}-linux
	cp -r `pwd`/deploy/* axis2c-bin-${PACKAGE_VERSION}-linux
	cp AUTHORS axis2c-bin-${PACKAGE_VERSION}-linux
	cp README axis2c-bin-${PACKAGE_VERSION}-linux
	cp ChangeLog axis2c-bin-${PACKAGE_VERSION}-linux		
	tar -cf - axis2c-bin-${PACKAGE_VERSION}-linux | gzip -c > axis2c-bin-${PACKAGE_VERSION}-linux.tar.gz
	rm -rf axis2c-bin-${PACKAGE_VERSION}-linux

install-data-hook:
	mkdir -p $(DESTDIR)$(docsdir)
	cp -r docs $(DESTDIR)$(docsdir)
	rm -rf `find $(DESTDIR)$(docsdir)/ -type d -name .svn`
	rm $(DESTDIR)$(docsdir)/README


