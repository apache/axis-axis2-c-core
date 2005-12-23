prgbindir=$(prefix)/bin/system_test
prgbin_PROGRAMS = test_om
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS) -g -O2 -pthread
test_om_SOURCES = test_om.c

test_om_LDADD   =   \
                    -L$(top_builddir)/install/lib \
                    -laxis2_om \
                    -laxis2_util \
                    -laxis2_parser \
                    -L$(LIBXML2_LIB) $(LIBXML2_LIBNAME) \
                    -L$(GUTHTHILA_LIB) $(GUTHTHILA_LIBNAME)
                    
INCLUDES = -I${CUTEST_HOME}/include -I$(top_builddir)/include \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/modules/util/src 
