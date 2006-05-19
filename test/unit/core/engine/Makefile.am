TESTS = engine_test
prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_engine.la
libtest_engine_la_SOURCES = engine_test.c test_conf.c
noinst_HEADERS=$(top_builddir)/test/unit/core/engine/*.h

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/clientapi \
            -I$(top_builddir)/modules/wsdl \
            -I${CUTEST_HOME}/include \
			@AXIOMINC@ \
			@UTILINC@ \
			@UTILINC@/platforms

check_PROGRAMS = engine_test
prgbin_PROGRAMS = engine_test
engine_test_SOURCES = main.c
engine_test_LDADD   =   libtest_engine.la  \
                     -L$(CUTEST_HOME)/lib \
                   	-laxis2_util \
				    -laxis2_unix \
					-laxis2_om \
					-laxis2_soap \
					-laxis2_parser \
					$(top_builddir)/modules/core/engine/libaxis2_engine.la \
					$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
                     -lcutest \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)
