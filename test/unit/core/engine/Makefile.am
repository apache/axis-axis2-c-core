TESTS = engine_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_engine.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_engine_la_SOURCES = engine_test.c test_conf.c
include_HEADERS=$(top_builddir)/test/unit/core/engine/*.h

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/wsdl \
            -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/platforms

check_PROGRAMS = engine_test
engine_test_SOURCES = main.c
engine_test_LDADD   =   libtest_engine.la  \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
                    -L$(AXIS2C_HOME)/lib \
                    -laxis2_util \
                    -laxis2_unix \
                    -laxis2_context \
                    -laxis2_description \
                    -laxis2_deployment \
                    -laxis2_engine \
                    -laxis2_wsdl \
                    -laxis2_phaseresolver \
                    -laxis2_om \
                    -laxis2_parser \
                    -laxis2_addr \
                    -L$(GUTHTHILA_LIB) $(GUTHTHILA_LIBNAME) \
                    -L$(LIBXML2_LIB) $(LIBXML2_LIBNAME)
