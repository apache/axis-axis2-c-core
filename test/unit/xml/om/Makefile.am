TESTS = om_test
prglibdir=$(prefix)/lib/unit_test
noinst_HEADERS = om_node_test.h
prglib_LTLIBRARIES = libtest_om.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_om_la_SOURCES = om_test.c om_node_test.c

                        
include_HEADERS=$(top_builddir)/test/unit/xml/om/*.h

INCLUDES = -I$(top_builddir)/include \
            -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/platforms

check_PROGRAMS = om_test
om_test_SOURCES = main.c
om_test_LDADD   =   libtest_om.la  \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
                    -L$(AXIS2C_HOME)/lib \
                    -laxis2_util \
                    -laxis2_om \
                    -laxis2_unix \
                    -laxis2_parser \
                    -L$(LIBXML2_LIB) $(LIBXML2_LIBNAME) \
                    -L$(GUTHTHILA_LIB) $(GUTHTHILA_LIBNAME)
                    
