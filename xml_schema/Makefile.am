TESTS =
SUBDIRS = src $(TESTDIR) 
include_HEADERS=$(top_builddir)/include/*.h
EXTRA_DIST = build.sh autogen.sh LICENSE samples
dist-hook:
	rm -rf `find $(distdir)/ -type d -name .svn`
	rm -rf `find $(distdir)/ -type d -name .deps`

                                 
