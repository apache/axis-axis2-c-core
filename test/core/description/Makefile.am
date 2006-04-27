TESTS = test_description
prgbindir=$(prefix)/bin/system_test
prgbin_PROGRAMS = test_description
SUBDIRS =
AM_CFLAGS = -g -O2 -pthread
test_description_SOURCES = test_description.c

test_description_LDADD   =  \
                    $(top_builddir)/modules/util/libaxis2_util.la \
					$(top_builddir)/modules/platforms/unix/libaxis2_unix.la \
					$(top_builddir)/modules/xml/om/libaxis2_om.la \
					$(top_builddir)/modules/xml/soap/libaxis2_soap.la \
					$(top_builddir)/modules/core/libaxis2_engine.la \
					$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
					$(top_builddir)/modules/xml/parser/libxml2/libaxis2_parser.la \
                    -lpthread \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)
                    
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/clientapi \
            -I$(top_builddir)/modules/platforms \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/include

