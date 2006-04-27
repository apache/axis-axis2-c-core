prgbindir=$(prefix)/bin/system_test
TESTS = test_context
check_PROGRAMS = test_context
prgbin_PROGRAMS = test_context
SUBDIRS =
AM_CFLAGS = -g -pthread
test_context_SOURCES = test_context.c


test_context_LDADD   =   \
                    $(top_builddir)/modules/util/libaxis2_util.la \
					$(top_builddir)/modules/platforms/unix/libaxis2_unix.la \
					$(top_builddir)/modules/xml/om/libaxis2_om.la \
					$(top_builddir)/modules/xml/soap/libaxis2_soap.la \
					$(top_builddir)/modules/core/libaxis2_engine.la \
					$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
					$(top_builddir)/modules/xml/parser/libxml2/libaxis2_parser.la \
			        $(GUTHTHILA_LIBS) \
			        $(LIBXML2_LIBS)

INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/xml/guththila/src \
            -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/platforms

