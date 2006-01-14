lib_LTLIBRARIES = libaxis2_http_transport.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_http_transport_la_SOURCES = axis2_http_worker.c \
                                     axis2_http_transport_utils.c\
                              	     axis2_simple_http_svr_conn.c\
                				     axis2_http_simple_request.c\
                				     axis2_http_simple_response.c\
				                     axis2_http_response_writer.c\
				                     axis2_http_request_line.c\
				                     axis2_http_status_line.c\
				                     axis2_http_header.c\
                                     axis2_url.c\
                                     axis2_http_out_transport_info.c\
                                     axis2_http_transport_sender.c\
                                     axis2_soap_over_http_sender.c\
                                     axis2_http_client.c

INCLUDES = -I$(top_builddir)/include \
           -I$(top_builddir)/modules/util \
           -I$(top_builddir)/modules/xml/parser\
	       -I$(top_builddir)/modules/wsdl\
	       -I$(top_builddir)/modules/core/transport\
	       -I$(top_builddir)/modules/core/transport/http \
           -I$(top_builddir)/modules/core/description \
            -I$(top_builddir)/modules/platforms

SUBDIRS = server
