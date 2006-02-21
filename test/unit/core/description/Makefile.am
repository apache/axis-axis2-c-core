TESTS = description_test
prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_description.la
libtest_description_la_SOURCES = description_test.c \
                                    test_op.c \
                                    test_svc.c \
                                    test_flow.c \
                                    test_handler_desc.c

noinst_HEADERS=$(top_builddir)/test/unit/core/description/*.h $(top_builddir)/test/unit/core/phaseresolver/*.h

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/clientapi \
            -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/platforms

prgbin_PROGRAMS = description_test
check_PROGRAMS = description_test
description_test_SOURCES = main.c
description_test_LDADD   =   libtest_description.la  \
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

