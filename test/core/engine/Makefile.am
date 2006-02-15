TESTS = test_engine
check_PROGRAMS = test_engine
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS) -g -pthread
test_engine_SOURCES = test_engine.c


test_engine_LDADD   =  -L$(AXIS2C_HOME)/lib \
	                -laxis2_util \
                    -laxis2_unix \
	                -laxis2_om \
	                -laxis2_soap \
	                -laxis2_engine \
	                -laxis2_wsdl \
	                -laxis2_parser \
	                -L$(GUTHTHILA_LIB) $(GUTHTHILA_LIBNAME) \
	                -L$(LIBXML2_LIB) $(LIBXML2_LIBNAME)

INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/xml/guththila \
            -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/modules/platforms

