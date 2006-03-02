TESTS = deployment_test
prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_deployment.la
libtest_deployment_la_SOURCES = deployment_test.c \
                                test_phases_info.c \
                                test_dep_engine.c

noinst_HEADERS=$(top_builddir)/test/unit/core/deployment/*.h


INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/platforms \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/clientapi \
            -I$(top_builddir)/modules/xml/parser \
            -I${CUTEST_HOME}/include

prgbin_PROGRAMS = deployment_test
check_PROGRAMS = deployment_test
deployment_test_SOURCES = main.c
deployment_test_LDADD   =   libtest_deployment.la  \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
                    -laxis2_util \
                    -laxis2_unix \
                    -laxis2_engine \
                    -laxis2_soap \
                    -laxis2_wsdl \
                    -laxis2_om \
                    -lpthread \
                    -laxis2_parser \
                    $(GUTHTHILA_LIBNAME) \
                    $(LIBXML2_LIBNAME)

