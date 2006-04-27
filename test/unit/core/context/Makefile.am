TESTS = context_test
prgbindir=$(prefix)/bin/unit_test
prglibdir=$(prefix)/lib/unit_test
prglib_LTLIBRARIES = libtest_context.la
libtest_context_la_SOURCES = context_test.c
noinst_HEADERS=$(top_builddir)/test/unit/core/context/*.h


INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I${CUTEST_HOME}/include

prgbin_PROGRAMS = context_test
check_PROGRAMS = context_test
context_test_SOURCES = main.c
context_test_LDADD   =   \
                        libtest_context.la  \
                        -L$(CUTEST_HOME)/lib \
                   		$(top_builddir)/modules/util/libaxis2_util.la \
				        $(top_builddir)/modules/platforms/unix/libaxis2_unix.la \
						$(top_builddir)/modules/xml/om/libaxis2_om.la \
						$(top_builddir)/modules/xml/soap/libaxis2_soap.la \
						$(top_builddir)/modules/core/libaxis2_engine.la \
						$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
						$(top_builddir)/modules/xml/parser/libxml2/libaxis2_parser.la \
                        -lcutest \
                        $(GUTHTHILA_LIBS) \
                        $(LIBXML2_LIBS)

