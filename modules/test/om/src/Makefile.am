prgbindir=$(prefix)/bin/system_test
prgbin_PROGRAMS = test_om
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS) -DLINUX=2 -D_REENTRANT -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -g -O2 -pthread
test_om_SOURCES = test_om.c

test_om_LDADD   =  $(LDFLAGS) $(top_builddir)/modules/xml/om/src/libom.la \
                    $(top_builddir)/modules/xml/guththila/src/libguththila.la
                    
INCLUDES = -I${CUTEST_HOME}/include -I$(top_builddir)/include \
            -I$(top_builddir)/modules/xml/guththila/src -I${APR_HOME}/include/apr-1
