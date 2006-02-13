TESTS =
lib_LTLIBRARIES = libaxis2_http_transport.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_http_transport_la_SOURCES = http_worker.c \
                                     http_transport_utils.c\
                				     http_simple_request.c\
                				     http_simple_response.c\
				                     http_request_line.c\
				                     http_status_line.c\
				                     http_header.c\
                                     url.c\
                                     http_out_transport_info.c\
                                     http_chunked_stream.c

INCLUDES = -I$(top_builddir)/include \
           -I$(top_builddir)/modules/util \
	       -I$(top_builddir)/modules/wsdl \
           -I$(top_builddir)/modules/xml/parser\
	       -I$(top_builddir)/modules/core/transport\
	       -I$(top_builddir)/modules/core/transport/http \
           -I$(top_builddir)/modules/core/description \
           -I$(top_builddir)/modules/platforms
