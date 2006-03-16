prgbindir=$(prefix)/bin/system_test
TESTS = test_http_transport
check_PROGRAMS = test_http_transport
prgbin_PROGRAMS = test_http_transport
SUBDIRS =
AM_CFLAGS = -g -pthread
test_http_transport_SOURCES = test_http_transport.c

test_http_transport_LDADD   =  \
                                $(LDFLAGS) \
								$(top_builddir)/modules/util/libaxis2_util.la \
								$(top_builddir)/modules/platforms/unix/libaxis2_unix.la \
								$(top_builddir)/modules/xml/om/libaxis2_om.la \
								$(top_builddir)/modules/xml/soap/libaxis2_soap.la \
								$(top_builddir)/modules/core/engine/libaxis2_engine.la \
								$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
								$(top_builddir)/modules/xml/parser/libxml2/libaxis2_parser.la \
								$(top_builddir)/modules/core/transport/http/sender/libaxis2_http_sender.la \
								$(LIBXML2_LIBS) \
                    			$(GUTHTHILA_LIBS)
                    
INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util\
            -I$(top_builddir)/modules/platforms 
