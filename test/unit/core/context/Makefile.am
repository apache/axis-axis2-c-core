TESTS = context_test
prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_context.la
libtest_context_la_SOURCES = context_test.c
noinst_HEADERS=$(top_builddir)/test/unit/core/context/*.h


INCLUDES = -I$(top_builddir)/include \
            -I${CUTEST_HOME}/include \
			@AXIOMINC@ \
			@UTILINC@ 

prgbin_PROGRAMS = context_test
check_PROGRAMS = context_test
context_test_SOURCES = main.c
context_test_LDADD   =   \
                        libtest_context.la  \
                        -L$(CUTEST_HOME)/lib \
                        -lcutest \
                   		-laxis2_util \
						-laxis2_axiom \
						-laxis2_parser \
						$(top_builddir)/modules/core/engine/libaxis2_engine.la \
						$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
                        $(GUTHTHILA_LIBS) \
                        $(LIBXML2_LIBS)

