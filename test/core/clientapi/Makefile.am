prgbindir=$(prefix)/bin/system_test/
TESTS = test_client test_clientapi
prgbin_PROGRAMS = test_client test_clientapi
check_PROGRAMS = test_client test_clientapi
SUBDIRS =
AM_CFLAGS = -g -pthread 
test_client_SOURCES = test_client.c
test_clientapi_SOURCES = test_clientapi.c

test_clientapi_LDADD   =  \
                    $(top_builddir)/modules/util/libaxis2_util.la \
					$(top_builddir)/modules/platforms/unix/libaxis2_unix.la \
					$(top_builddir)/modules/xml/om/libaxis2_om.la \
					$(top_builddir)/modules/xml/soap/libaxis2_soap.la \
					$(top_builddir)/modules/core/engine/libaxis2_engine.la \
					$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
                    $(top_builddir)/modules/xml/parser/libxml2/libaxis2_parser.la \
					$(top_builddir)/modules/core/transport/http/sender/libaxis2_http_sender.la \
	                -lpthread \
	                $(LIBXML2_LIBS) \
	                $(GUTHTHILA_LIBS)

test_client_LDADD   =  \
                    $(top_builddir)/modules/util/libaxis2_util.la \
					$(top_builddir)/modules/platforms/unix/libaxis2_unix.la \
					$(top_builddir)/modules/xml/om/libaxis2_om.la \
					$(top_builddir)/modules/xml/soap/libaxis2_soap.la \
					$(top_builddir)/modules/core/engine/libaxis2_engine.la \
					$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
                    $(top_builddir)/modules/xml/parser/libxml2/libaxis2_parser.la \
					$(top_builddir)/modules/core/transport/http/sender/libaxis2_http_sender.la \
	                -lpthread \
	                $(LIBXML2_LIBS) \
	                $(GUTHTHILA_LIBS) 

INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/core/context \
            -I$(top_builddir)/modules/core/phaseresolver \
            -I$(top_builddir)/modules/core/deployment \
            -I$(top_builddir)/modules/core/transport \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/modules/core/clientapi \
            -I$(top_builddir)/modules/xml/parser \
            -I$(top_builddir)/modules/platforms

