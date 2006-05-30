prgbindir=$(prefix)/bin/system_test
TESTS = test_context
check_PROGRAMS = test_context
prgbin_PROGRAMS = test_context
SUBDIRS =
AM_CFLAGS = -g -pthread
test_context_SOURCES = test_context.c


test_context_LDADD   =   \
                    -laxis2_util \
					-laxis2_axiom \
					-laxis2_parser \
					$(top_builddir)/modules/core/engine/libaxis2_engine.la \
					$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
			        $(GUTHTHILA_LIBS) \
			        $(LIBXML2_LIBS)

INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/xml/guththila/src \
            -I$(top_builddir)/include \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
			@AXIOMINC@ \
			@UTILINC@ 

