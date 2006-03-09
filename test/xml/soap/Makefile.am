TESTS = test_soap
prgbindir=$(prefix)/bin/system_test
prgbin_PROGRAMS = test_soap
check_PROGRAMS = test_soap
SUBDIRS =
AM_CFLAGS = -g -O2 -pthread
test_soap_SOURCES = test_soap.c

test_soap_LDADD   =   \
                    -laxis2_om \
                    -laxis2_soap \
                    -laxis2_util \
                    -laxis2_parser \
                    -laxis2_unix \
                    -laxis2_unix \
                    $(LIBXML2_LIBS) \
                    $(GUTHTHILA_LIBS)
                    
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/platforms
