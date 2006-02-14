TESTS =
prgbindir=$(prefix)/bin/unit_test
prgbin_PROGRAMS = main_unit_test_suite
SUBDIRS = util core xml wsdl
AM_CPPFLAGS = $(CPPFLAGS)
main_unit_test_suite_SOURCES = main.c

main_unit_test_suite_LDADD   =   \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
                    -L$(top_builddir)/test/unit/core/description/.libs -ltest_description \
                    -L$(top_builddir)/test/unit/core/engine/.libs -ltest_engine \
                    -L$(top_builddir)/test/unit/core/context/.libs -ltest_context \
                    -L$(top_builddir)/test/unit/core/phaseresolver/.libs -ltest_phaseresolver \
                    -L$(top_builddir)/test/unit/util/.libs -ltest_util \
                    -L$(top_builddir)/test/unit/wsdl/.libs -ltest_wsdl \
                    -L$(top_builddir)/test/unit/xml/om/.libs -ltest_om \
                    -L$(AXIS2C_HOME)/lib \
                    -laxis2_util \
                    -laxis2_unix \
                    -laxis2_om \
                    -lpthread \
                    -laxis2_engine \
                    -laxis2_soap \
                    -laxis2_wsdl \
                    -laxis2_parser \
                    -L$(GUTHTHILA_LIB) $(GUTHTHILA_LIBNAME) \
                    -L$(LIBXML2_LIB) $(LIBXML2_LIBNAME)
                    

INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/clientapi \
            -I$(top_builddir)/modules/platforms
