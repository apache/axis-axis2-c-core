TESTS =
SUBDIRS = src $(TESTDIR) samples
include_HEADERS=$(top_builddir)/include/*.h
EXTRA_DIST = build.sh autogen.sh LICENSE
dist-hook:
	rm -rf `find $(distdir)/ -type d -name .svn`
	rm -rf `find $(distdir)/ -type d -name .deps`

                                 
