prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_deployment.la
AM_CPPFLAGS = $(CPPFLAGS)
libtest_deployment_la_SOURCES = deployment_test.c \
                                test_phases_info.c \
                                test_dep_engine.c

include_HEADERS=$(top_builddir)/test/unit/core/deployment/*.h


INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/platforms \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/xml/parser \
            -I${CUTEST_HOME}/include

prgbin_PROGRAMS = deployment_test
deployment_test_SOURCES = main.c
deployment_test_LDADD   =   libtest_deployment.la  \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
                    -L$(top_builddir)/install/lib \
                    -laxis2_util \
                    -laxis2_context \
                    -laxis2_deployment \
                    -laxis2_description \
                    -laxis2_engine \
                    -laxis2_wsdl \
                    -laxis2_phaseresolver \
                    -laxis2_om \
                    -laxis2_parser \
                    -laxis2_addr \
                    -L$(GUTHTHILA_LIB) $(GUTHTHILA_LIBNAME) \
                    -L$(LIBXML2_LIB) $(LIBXML2_LIBNAME)

