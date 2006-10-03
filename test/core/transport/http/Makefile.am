prgbindir=$(prefix)/bin/system_test
TESTS = test_http_transport
check_PROGRAMS = test_http_transport
prgbin_PROGRAMS = test_http_transport
SUBDIRS =
AM_CFLAGS = -g -pthread
test_http_transport_SOURCES = test_http_transport.c

test_http_transport_LDADD   =  \
                                $(LDFLAGS) \
		                    ../../../../util/src/libaxis2_util.la \
       						../../../../axiom/src/om/libaxis2_axiom.la \
		                    ../../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
		                    ../../../../woden/src/wsdl20/libwoden.la \
		                    ../../../../xml_schema/src/libaxis2_xml_schema.la \
							$(top_builddir)/modules/core/engine/libaxis2_engine.la \
							$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
							$(top_builddir)/modules/core/transport/http/sender/libaxis2_http_sender.la \
							$(LIBXML2_LIBS) \
                    		-L$(top_builddir)$(GUTHTHILA_LIBS)
                    
INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/include \
            -I ../../../../util/include \
            -I ../../../../axiom/include

