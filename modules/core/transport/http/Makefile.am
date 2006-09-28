SUBDIRS = 
noinst_LTLIBRARIES = libaxis2_http.la

libaxis2_http_la_SOURCES =	http_worker.c \
                            http_transport_utils.c\
                			http_simple_request.c\
                			http_simple_response.c\
				            http_request_line.c\
				            http_status_line.c\
				            http_header.c\
                            http_out_transport_info.c\
                            http_chunked_stream.c

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/wsdl \
            -I$(top_builddir)/modules/core/engine \
            -I$(top_builddir)/xml_schema/include \
            -I$(top_builddir)/util/include \
            -I$(top_builddir)/axiom/include
