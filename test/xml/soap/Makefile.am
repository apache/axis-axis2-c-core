prgbindir=$(prefix)/bin/system_test
prgbin_PROGRAMS = test_soap
SUBDIRS =
AM_CPPFLAGS = $(CPPFLAGS) -g -O2 -pthread
test_soap_SOURCES = test_soap.c

test_soap_LDADD   =   \
                    -L$(AXIS2C_HOME)/lib \
                    -laxis2_om \
                    -laxis2_soap \
                    -laxis2_util \
                    -laxis2_parser \
                    -laxis2_unix \
                    -laxis2_unix \
                    -L$(LIBXML2_LIB) $(LIBXML2_LIBNAME) \
                    -L$(GUTHTHILA_LIB) $(GUTHTHILA_LIBNAME)
                    
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/platforms
