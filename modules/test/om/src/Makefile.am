prgbindir=$(prefix)/bin/system_test
prgbin_PROGRAMS = test_om
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS) -g -O2 -pthread
test_om_SOURCES = test_om.c

#test_om_LDADD   =  $(LDFLAGS) $(top_builddir)/modules/xml/om/src/libaxis2_om.la \
#                    $(top_builddir)/modules/xml/guththila/src/libguththila.la

test_om_LDADD = $(LDFLAGS) -L$(top_builddir)/modules/xml/om/src/.libs -laxis2_om \
                    -L$(top_builddir)/modules/xml/guththila/src/.libs -lguththila \
                    -L$(top_builddir)/modules/util/src/.libs -laxis2_util

INCLUDES = -I${CUTEST_HOME}/include -I$(top_builddir)/include \
            -I$(top_builddir)/modules/xml/guththila/src
