TESTS =
lib_LTLIBRARIES = libaxis2_http_sender.la
libaxis2_http_sender_la_SOURCES = http_transport_sender.c\
                                  soap_over_http_sender.c\
                                  rest_sender.c\
                                  http_client.c
if AXIS2_SSL_ENABLED
libaxis2_http_sender_la_SOURCES += ssl/ssl_stream.c\
                                   ssl/ssl_utils.c
libaxis2_http_sender_la_LIBADD = -lssl\
                                 -lcrypto
endif

INCLUDES = -I$(top_builddir)/include \
	       -I$(top_builddir)/modules/wsdl\
	       -I$(top_builddir)/modules/core/transport\
	       -I$(top_builddir)/modules/core/transport/http \
           -I$(top_builddir)/modules/core/description \
           -I$(top_builddir)/modules/core/context \
           -I$(top_builddir)/modules/core/phaseresolver \
           -I$(top_builddir)/modules/core/engine \
           -I$(top_builddir)/modules/core/deployment \
			@AXIOMINC@ \
			@UTILINC@ \
			@UTILINC@/platforms
