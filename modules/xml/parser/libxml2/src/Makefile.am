lib_LTLIBRARIES = libaxis2_libxml2.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_libxml2_la_SOURCES = libxml2.c

libaxis2_libxml2_la_LIBADD = -L${LIBXML2_LIB} -lxml2 $(LDFLAGS)
INCLUDES = -I$(top_builddir)/include -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/util/src \
            -I$(top_builddir)/modules/xml/parser \
            -I${LIBXML2_INCLUDE}
