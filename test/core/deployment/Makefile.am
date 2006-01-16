prgbindir=$(prefix)/bin/system_test
prgbin_PROGRAMS = test_deployment
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS) -g -O2 -pthread
test_deployment_SOURCES = test_deployment.c

test_deployment_LDADD   =  -L$(AXIS2C_INSTALL)/lib \
					-laxis2_util \
                    -laxis2_context \
                    -laxis2_description \
                    -laxis2_deployment \
                    -laxis2_phaseresolver \
                    -laxis2_om \
                    -laxis2_wsdl \
                    -laxis2_engine \
                    -laxis2_parser \
                    -laxis2_addr \
                    -L$(GUTHTHILA_LIB) $(GUTHTHILA_LIBNAME) \
                    -L$(LIBXML2_LIB) $(LIBXML2_LIBNAME)
                    
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/platforms \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/include

