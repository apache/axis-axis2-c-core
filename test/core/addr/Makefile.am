prgbindir=$(prefix)/bin/system_test
prgbin_PROGRAMS = test_addr
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS) -g -pthread
test_addr_SOURCES = test_addr.c


test_addr_LDADD   =  -L$(AXIS2C_HOME)/lib \
	                -laxis2_description \
	                -laxis2_context \
	                -laxis2_util \
	                -laxis2_deployment \
	                -laxis2_om \
	                -laxis2_engine \
	                -laxis2_wsdl \
	                -laxis2_parser \
	                -laxis2_phaseresolver \
	                -laxis2_addr \
	                -L$(GUTHTHILA_LIB) $(GUTHTHILA_LIBNAME) \
	                -L$(LIBXML2_LIB) $(LIBXML2_LIBNAME)

INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/xml/guththila \
            -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/platforms

