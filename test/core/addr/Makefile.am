TESTS = test_addr
check_PROGRAMS = test_addr
bin_PROGRAMS = test_addr
SUBDIRS =
AM_CFLAGS = -g -pthread
test_addr_SOURCES = test_addr.c


test_addr_LDADD   =  \
	                -laxis2_util \
                    -laxis2_unix \
	                -laxis2_om \
	                -laxis2_soap \
	                -laxis2_engine \
	                -laxis2_wsdl \
	                -laxis2_parser \
	                $(GUTHTHILA_LIBNAME) \
	                $(LIBXML2_LIBNAME)

INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/xml/guththila \
            -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/platforms

