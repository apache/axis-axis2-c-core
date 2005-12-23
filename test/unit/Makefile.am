prgbindir=$(prefix)/bin/unit_test
prgbin_PROGRAMS = main_unit_test_suite
SUBDIRS = util core xml
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
                    -L$(top_builddir)/test/unit/xml/om/.libs -ltest_om \
                    -L$(top_builddir)/install/lib \
                    -laxis2_util \
                    -laxis2_om \
                    -laxis2_description \
                    -laxis2_engine \
                    -laxis2_context \
                    -laxis2_phaseresolver \
                    -laxis2_wsdl \
                    -laxis2_parser \
                    -laxis2_addr \
                    -L$(GUTHTHILA_LIB) $(GUTHTHILA_LIBNAME) \
                    -L$(LIBXML2_LIB) $(LIBXML2_LIBNAME)
                    

INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util/src \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/core/description/src \
            -I$(top_builddir)/modules/wsdl/src
