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
	./autogen.sh
	./configure --prefix=${AXIS2C_HOME} --enable-tests=yes --enable-trace=yes  --with-apache2=/usr/include/apache2 --with-apr=/usr/include/apr-0 --enable-diclient=no --enable-static=no --enable-openssl=no --with-axis2_util=${AXIS2C_HOME}/include --with-axiom=${AXIS2C_HOME}/include
	make
	make install
	mkdir axis2c-bin-${PACKAGE_VERSION}-linux
	cp -r ${AXIS2C_HOME}/* axis2c-bin-${PACKAGE_VERSION}-linux
	tar cf axis2c-bin-${PACKAGE_VERSION}-linux.tar.gz axis2c-bin-${PACKAGE_VERSION}-linux
	rm -rf axis2c-bin-${PACKAGE_VERSION}-linux

