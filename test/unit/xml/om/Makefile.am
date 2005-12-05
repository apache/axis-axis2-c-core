prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_om.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_om_la_SOURCES = om_test.c om_node_test.c

                        
include_HEADERS=$(top_builddir)/test/unit/xml/om/*.h

INCLUDES = -I$(top_builddir)/include \
            -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/util/src

prgbin_PROGRAMS = om_test
om_test_SOURCES = main.c
om_test_LDADD   =  $(LDFLAGS) libtest_om.la  \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
                    -L$(top_builddir)/install/lib \
                    -laxis2_util \
                    -laxis2_context \
                    -laxis2_description \
                    -laxis2_engine \
                    -laxis2_wsdl \
                    -laxis2_phaseresolver \
                    -laxis2_om \
                    -laxis2_guththila \
                    -lguththila
