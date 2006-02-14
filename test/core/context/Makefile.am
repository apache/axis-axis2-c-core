TESTS = test_context
check_PROGRAMS = test_context
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS) -g -pthread
test_context_SOURCES = test_context.c


test_context_LDADD   =  -L$(AXIS2C_HOME)/lib \
                    -laxis2_util \
                    -laxis2_unix \
                    -laxis2_om \
			        -laxis2_engine \
			        -laxis2_parser \
			        -laxis2_wsdl \
			        -L$(GUTHTHILA_LIB) $(GUTHTHILA_LIBNAME) \
			        -L$(LIBXML2_LIB) $(LIBXML2_LIBNAME)

INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/modules/xml/guththila/src \
            -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/platforms

