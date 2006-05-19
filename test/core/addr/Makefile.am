TESTS = test_addr
prgbindir=$(prefix)/bin/system_test
check_PROGRAMS = test_addr
prgbin_PROGRAMS = test_addr
SUBDIRS =
AM_CFLAGS = -g -pthread
test_addr_SOURCES = test_addr.c

test_addr_LDADD   =  \
					-laxis2_util \
					-laxis2_unix \
					-laxis2_om \
					-laxis2_soap \
					-laxis2_parser \
					$(top_builddir)/modules/core/engine/libaxis2_engine.la \
					$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
	                $(GUTHTHILA_LIBS) \
	                $(LIBXML2_LIBS)

INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/xml/guththila \
            -I$(top_builddir)/include \
			@AXIOMINC@ \
			@UTILINC@ \
			@UTILINC@/platforms

