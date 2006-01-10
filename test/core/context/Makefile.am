prgbindir=$(prefix)/bin/system_test
prgbin_PROGRAMS = test_context
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS) -g -O2 -pthread
test_context_SOURCES = test_context.c


test_context_LDADD   =  -L$(top_builddir)/install/lib \
			        -laxis2_description \
			        -laxis2_context \
                    -laxis2_util \
                    -laxis2_om \
			        -laxis2_engine \
	                -laxis2_deployment \
			        -laxis2_wsdl \
			        -laxis2_parser \
			        -laxis2_phaseresolver \
			        -laxis2_addr \
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

