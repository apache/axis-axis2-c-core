TESTS =
lib_LTLIBRARIES = libaxis2_http_sender.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_http_sender_la_SOURCES = http_transport_sender.c\
                                  soap_over_http_sender.c\
                                  http_client.c

INCLUDES = -I$(top_builddir)/include \
           -I$(top_builddir)/modules/util \
           -I$(top_builddir)/modules/xml/parser\
	       -I$(top_builddir)/modules/wsdl\
	       -I$(top_builddir)/modules/core/transport\
	       -I$(top_builddir)/modules/core/transport/http \
           -I$(top_builddir)/modules/core/description \
           -I$(top_builddir)/modules/core/context \
           -I$(top_builddir)/modules/core/phaseresolver \
           -I$(top_builddir)/modules/core/engine \
           -I$(top_builddir)/modules/core/deployment \
           -I$(top_builddir)/modules/platforms
