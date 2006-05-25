TESTS =
datadir=$(prefix)
SUBDIRS = src $(TESTDIR) include
include_HEADERS=$(top_builddir)/include/*.h
data_DATA= INSTALL README AUTHORS NEWS CREDITS LICENSE COPYING
EXTRA_DIST = build.sh autogen.sh CREDITS LICENSE maven.xml project.xml
dist-hook:
	rm -rf `find $(distdir)/ -type d -name .svn`
	rm -rf `find $(distdir)/ -type d -name .deps`
