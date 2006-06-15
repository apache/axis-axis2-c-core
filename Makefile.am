datadir=$(prefix)
logsdir=$(prefix)/logs
SUBDIRS = modules $(TESTDIR) include ides
include_HEADERS=$(top_builddir)/include/*.h
data_DATA= samples/server/axis2.xml INSTALL README AUTHORS NEWS CREDITS LICENSE COPYING
logs_DATA=samples/server/axis2.log
EXTRA_DIST = build.sh autogen.sh CREDITS LICENSE samples/server/axis2.log samples build
dist-hook:
	rm -rf `find $(distdir)/ -type d -name .svn`
	rm -rf `find $(distdir)/ -type d -name .deps`

bindist: $(bin_PROGRAMS)
	rm -rf axis2c-bin-${PACKAGE_VERSION}-linux
	sh bindist.sh
	mkdir axis2c-bin-${PACKAGE_VERSION}-linux
	cp -r ${AXIS2C_HOME}/* axis2c-bin-${PACKAGE_VERSION}-linux
	tar -cf - axis2c-bin-${PACKAGE_VERSION}-linux | gzip -c > axis2c-bin-${PACKAGE_VERSION}-linux.tar.gz
	rm -rf axis2c-bin-${PACKAGE_VERSION}-linux
