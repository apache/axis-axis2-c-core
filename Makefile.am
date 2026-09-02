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
# AXIS2C-1338: Use proper installation directories following standard conventions
# Data files go to $(datadir)/axis2c, docs to $(docdir), tools to libexec
axis2cdatadir=$(datadir)/axis2c
logsdir=$(localstatedir)/log/axis2c
docsdir=$(docdir)
includedir=$(prefix)/include/axis2-${PACKAGE_VERSION}
wsdl2cdir=$(libexecdir)/axis2c/wsdl2c


pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = axis2c.pc

# Conditionally include gtest only if it has a proper Makefile
if GTEST_AVAILABLE
GTEST_SUBDIR = gtest
else
GTEST_SUBDIR =
endif

# HTTP/2 uses pure JSON architecture while HTTP/1.1 maintains SOAP support via axiom
# tcpmon removed (January 2026) - TCP transport obsolete, use HTTP/2 instead
SUBDIRS = $(GTEST_SUBDIR) util $(GUTHTHILA_DIR) axiom neethi src include ides tools/md5 tools/codegen $(TESTDIR)
include_HEADERS=$(top_srcdir)/include/*.h
# Install configuration files and documentation
# For in-tree builds, skip installing files that are already in the correct location
# to avoid "file and destination are the same" errors during development builds
axis2cdata_DATA= samples/server/axis2.xml
logs_DATA=
docs_DATA=
wsdl2c_DATA=tools/codegen/javatool/WSDL2C.sh tools/codegen/javatool/README
# Conditionally include gtest in distribution only if it's available
if GTEST_AVAILABLE
GTEST_EXTRA_DIST = gtest
else
GTEST_EXTRA_DIST =
endif

EXTRA_DIST = CREDITS LICENSE build axis2c_build.sh guththila tools AUTHORS NOTICE tools/codegen/javatool/README  tools/codegen/javatool/WSDL2C.bat  tools/codegen/javatool/WSDL2C.sh docs docs/README samples $(GTEST_EXTRA_DIST)

# "." is not in SUBDIRS, so automake runs check-am -- and this hook -- after it
# has recursed into every subdirectory. That puts the banner below the last
# "All tests passed!", which is the line it exists to contradict.
if TESTS_WITHOUT_GTEST
check-local:
	@echo ""
	@echo "========================================================================"
	@echo "WARNING: nothing was tested. No gtest suite was built."
	@echo "========================================================================"
	@echo "--enable-tests was given but --with-gtest was not, so every gtest"
	@echo "suite compiled out. Each 'Testsuite summary' above reports TOTAL: 0,"
	@echo "and the 'All tests passed!' line passed over zero tests. The gtest"
	@echo "suites are what exercise the library."
	@echo ""
	@echo "Reconfigure with the googletest SOURCE tree -- a distro binary"
	@echo "package will not do -- and rebuild from clean, because objects do not"
	@echo "record the flags they were built with:"
	@echo ""
	@echo '    ./configure --prefix=$$PWD/deploy --enable-tests --enable-json=yes --with-gtest=/usr/src/googletest/googletest'
	@echo "    make clean && make && make install"
	@echo '    (cd samples && AXIS2C_HOME=$$PWD/../deploy sh build.sh)'
	@echo '    AXIS2C_HOME=$$PWD/deploy LD_LIBRARY_PATH=$$PWD/deploy/lib make check'
	@echo ""
	@echo "Expect 44 tests. Build the samples too: without them there is no echo"
	@echo "service for the integration tests to call, and they will fail."
	@echo "========================================================================"
else
check-local:
	@:
endif

dist-hook:
	rm -rf $(distdir)/gtest
	rm -rf `find $(distdir)/ -type d -name .git`
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
	rm -rf $(distdir)/tools/md5/src/md5
	find $(distdir) -name "makefile" | xargs sed -i "s/\/WX//g"
	find $(distdir) -name "*.gcda" -delete
	find $(distdir) -name "*.gcno" -delete
	find $(distdir) -path "*/gtest/gtest-all.cc" -delete
	find $(distdir) -path "*/gtest/gtest_main.cc" -delete

bindist: $(bin_PROGRAMS)
	rm -rf axis2c-bin-${PACKAGE_VERSION}-linux
	mkdir axis2c-bin-${PACKAGE_VERSION}-linux
	cp -r `pwd`/deploy/* axis2c-bin-${PACKAGE_VERSION}-linux
	cp AUTHORS axis2c-bin-${PACKAGE_VERSION}-linux
	cp README axis2c-bin-${PACKAGE_VERSION}-linux
	cp ChangeLog axis2c-bin-${PACKAGE_VERSION}-linux		
	tar -cf - axis2c-bin-${PACKAGE_VERSION}-linux | gzip -c > axis2c-bin-${PACKAGE_VERSION}-linux.tar.gz
	rm -rf axis2c-bin-${PACKAGE_VERSION}-linux

# Install documentation and license files conditionally to prevent "same file" errors
# during in-tree development builds. Uses test -ef to check if files are identical.
# Override distclean-recursive to handle subdirectories that may lack distclean targets gracefully
# This prevents the recurring "No rule to make target 'distclean'" errors
distclean-recursive:
	@echo "Performing robust recursive distclean..."
	@for subdir in $(SUBDIRS); do \
		if [ -f $$subdir/Makefile ]; then \
			echo "Running distclean in $$subdir"; \
			$(MAKE) -C $$subdir distclean || echo "Warning: distclean failed in $$subdir, continuing..."; \
		elif [ -f $$subdir/Makefile.in ]; then \
			echo "$$subdir has Makefile.in but no Makefile - cleaning manually"; \
			rm -f $$subdir/Makefile $$subdir/config.log $$subdir/config.status $$subdir/config.h || true; \
			rm -rf $$subdir/.deps $$subdir/.libs $$subdir/_libs || true; \
		else \
			echo "$$subdir lacks proper autotools setup - skipping distclean"; \
		fi; \
	done

install-data-hook:
	mkdir -p $(DESTDIR)$(docsdir)
	cp -r docs $(DESTDIR)$(docsdir)
	rm -rf `find $(DESTDIR)$(docsdir)/ -type d -name .git`
	rm -f $(DESTDIR)$(docsdir)/README
	@for file in README INSTALL CREDITS COPYING NEWS NOTICE AUTHORS; do \
		if [ ! "$(abs_srcdir)/$$file" -ef "$(DESTDIR)$(datadir)/$$file" ]; then \
			$(INSTALL_DATA) "$(srcdir)/$$file" "$(DESTDIR)$(datadir)/$$file"; \
		else \
			echo "Skipping $$file - already in correct location"; \
		fi; \
	done
	@echo "Creating axis2.xml symlink at $(DESTDIR)$(prefix) for deployment engine compatibility"
	@rm -f $(DESTDIR)$(prefix)/axis2.xml 2>/dev/null || true
	@cd $(DESTDIR)$(prefix) && $(LN_S) share/axis2c/axis2.xml axis2.xml 2>/dev/null || \
		echo "Note: Could not create axis2.xml symlink (may already exist or different filesystem)"
	@echo "Note: axis2.xml has just been REPLACED with the shipped default."
	@echo "      $(DESTDIR)$(prefix)/axis2.xml is a symlink to share/axis2c/axis2.xml,"
	@echo "      and that file is reinstalled by every 'make install' -- so any local"
	@echo "      edits (enabled transports, attachmentDIR, ports) are gone. Reapply"
	@echo "      them after installing, or keep the configuration outside the prefix."
	@echo ""
	@echo "========================================================================"
	@echo "CRITICAL: Complete Apache Module & Library Deployment Required!"
	@echo "========================================================================"
	@echo "make install only updates some libraries. Apache needs ALL components:"
	@echo ""
	@echo "# Install updated receivers library (contains ServiceClass parameter fixes)"
	@echo "sudo cp src/core/receivers/.libs/libaxis2_receivers.a /usr/local/axis2c/lib/libaxis2_receivers.a"
	@echo ""
	@echo "# Install updated Apache module"
	@echo "sudo cp src/core/transport/http/server/apache2/.libs/libmod_axis2.so /usr/local/apache2/modules/mod_axis2.so"
	@echo ""
	@echo "# Update the module in the Axis2/C lib directory"
	@echo "sudo cp src/core/transport/http/server/apache2/.libs/libmod_axis2.so /usr/local/axis2c/lib/libmod_axis2.so"
	@echo ""
	@echo "# CRITICAL: Restart Apache to load new libraries (running processes cache old versions)"
	@echo "sudo systemctl restart apache2-custom"
	@echo ""
	@echo "Without ALL these steps, Apache will continue using old library versions"
	@echo "and ServiceClass parameter fixes will NOT take effect!"
	@echo "========================================================================"


