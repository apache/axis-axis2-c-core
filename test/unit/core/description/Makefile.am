prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_description.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_description_la_SOURCES = description_test.c \
                                    test_op.c \
                                    test_svc.c

include_HEADERS=$(top_builddir)/test/unit/core/description/*.h

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/wsdl \
            -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/platforms

prgbin_PROGRAMS = description_test
description_test_SOURCES = main.c
description_test_LDADD   =   libtest_description.la  \
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
                    -laxis2_parser \
                    -laxis2_addr \
                    -L$(GUTHTHILA_LIB) $(GUTHTHILA_LIBNAME) \
                    -L$(LIBXML2_LIB) $(LIBXML2_LIBNAME)

