TESTS = context_test
prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_context.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_context_la_SOURCES = context_test.c
noinst_HEADERS=$(top_builddir)/test/unit/core/context/*.h


INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I${CUTEST_HOME}/include

prgbin_PROGRAMS = context_test
check_PROGRAMS = context_test
context_test_SOURCES = main.c
context_test_LDADD   =   libtest_context.la  \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
                    -L$(AXIS2C_HOME)/lib \
                    -laxis2_util \
                    -laxis2_unix \
                    -laxis2_engine \
                    -laxis2_wsdl \
                    -laxis2_om \
                    -laxis2_soap \
                    -laxis2_parser \
                    -L$(GUTHTHILA_LIB) $(GUTHTHILA_LIBNAME) \
                    -L$(LIBXML2_LIB) $(LIBXML2_LIBNAME)

