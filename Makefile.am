datadir=$(prefix)
scriptsdir=$(prefix)/bin
logsdir=$(prefix)/logs
docsdir=$(prefix)
includedir=$(prefix)/include/axis2-1.3.0


pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = axis2c.pc

SUBDIRS = util $(GUTHTHILA_DIR) axiom neethi src $(TESTDIR) include ides tools/tcpmon tools/md5
include_HEADERS=$(top_builddir)/include/*.h
data_DATA= samples/server/axis2.xml README \
	INSTALL CREDITS COPYING NEWS NOTICE 
scripts_DATA=tools/codegen/javatool/WSDL2C.sh
logs_DATA=
docs_DATA= 
EXTRA_DIST = CREDITS LICENSE build axis2c_build.sh guththila tools AUTHORS NOTICE 

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
	sh dist.sh

bindist: $(bin_PROGRAMS)
	rm -rf axis2c-bin-${PACKAGE_VERSION}-linux
	sh bindist.sh
	mkdir axis2c-bin-${PACKAGE_VERSION}-linux
	cp -r `pwd`/deploy/* axis2c-bin-${PACKAGE_VERSION}-linux
	cp AUTHORS axis2c-bin-${PACKAGE_VERSION}-linux
	tar -cf - axis2c-bin-${PACKAGE_VERSION}-linux | gzip -c > axis2c-bin-${PACKAGE_VERSION}-linux.tar.gz
	rm -rf axis2c-bin-${PACKAGE_VERSION}-linux

install-data-hook:
	cp -r xdocs/docs $(docsdir)
