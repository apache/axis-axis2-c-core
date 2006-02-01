prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_phaseresolver.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_phaseresolver_la_SOURCES = phaseresolver_test.c \
                                    test_resolver.c \
                                    test_phase_holder.c

include_HEADERS=$(top_builddir)/test/unit/core/phaseresolver/*.h

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/wsdl \
            -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/platforms

prgbin_PROGRAMS = phaseresolver_test
phaseresolver_test_SOURCES = main.c
phaseresolver_test_LDADD   =   libtest_phaseresolver.la  \
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

