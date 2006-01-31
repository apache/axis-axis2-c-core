prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_wsdl.la
AM_CPPFLAGS = $(CPPFLAGS)

libtest_wsdl_la_SOURCES = wsdl_test.c test_wsdl_component.c

include_HEADERS=$(top_builddir)/test/unit/wsdl/*.h

INCLUDES = -I$(top_builddir)/include \
            -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/platforms


prgbin_PROGRAMS = wsdl_test
wsdl_test_SOURCES = main.c

wsdl_test_LDADD   =   \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
                     $(top_builddir)/test/unit/wsdl/libtest_wsdl.la \
                    -L$(AXIS2C_HOME)/lib \
                    -laxis2_util \
                    -laxis2_wsdl \
					-laxis2_unix \
                    -laxis2_phaseresolver \
                    -laxis2_context \
                    -laxis2_addr \
                    -laxis2_description

