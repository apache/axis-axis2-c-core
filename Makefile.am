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

SUBDIRS = $(GTEST) util $(GUTHTHILA_DIR) axiom neethi src include ides tools/tcpmon tools/md5 tools/codegen $(TESTDIR)
include_HEADERS=$(top_srcdir)/include/*.h
# Install configuration files and documentation
# For in-tree builds, skip installing files that are already in the correct location
# to avoid "file and destination are the same" errors during development builds
data_DATA= samples/server/axis2.xml
logs_DATA=
docs_DATA= 
wsdl2c_DATA=tools/codegen/javatool/WSDL2C.sh tools/codegen/javatool/README
EXTRA_DIST = CREDITS LICENSE build axis2c_build.sh guththila tools AUTHORS NOTICE tools/codegen/javatool/README  tools/codegen/javatool/WSDL2C.bat  tools/codegen/javatool/WSDL2C.sh docs docs/README samples gtest

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
	rm -rf `find $(distdir)/ -type f -name config.status`
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

# Install documentation and license files conditionally to prevent "same file" errors
# during in-tree development builds. Uses test -ef to check if files are identical.
install-data-hook:
	mkdir -p $(DESTDIR)$(docsdir)
	cp -r docs $(DESTDIR)$(docsdir)
	rm -rf `find $(DESTDIR)$(docsdir)/ -type d -name .svn`
	rm $(DESTDIR)$(docsdir)/README
	@for file in README INSTALL CREDITS COPYING NEWS NOTICE AUTHORS; do \
		if [ ! "$(abs_srcdir)/$$file" -ef "$(DESTDIR)$(datadir)/$$file" ]; then \
			$(INSTALL_DATA) "$(srcdir)/$$file" "$(DESTDIR)$(datadir)/$$file"; \
		else \
			echo "Skipping $$file - already in correct location"; \
		fi; \
	done


