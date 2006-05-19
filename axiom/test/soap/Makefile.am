TESTS = test_soap
prgbindir=$(prefix)/bin/system_test
prgbin_PROGRAMS = test_soap
check_PROGRAMS = test_soap
SUBDIRS =
AM_CFLAGS = -g -O2 -pthread
test_soap_SOURCES = test_soap.c

test_soap_LDADD   =   \
                    $(top_builddir)/modules/util/libaxis2_util.la \
					$(top_builddir)/modules/platforms/unix/libaxis2_unix.la \
					$(top_builddir)/modules/xml/om/libaxis2_om.la \
					$(top_builddir)/modules/xml/soap/libaxis2_soap.la \
					$(top_builddir)/modules/xml/parser/libxml2/libaxis2_parser.la \
                    $(LIBXML2_LIBS) \
                    $(GUTHTHILA_LIBS)
                    
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/platforms
