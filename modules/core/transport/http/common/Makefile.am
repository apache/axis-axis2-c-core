lib_LTLIBRARIES = libaxis2_http_common.la

libaxis2_http_common_la_LIBADD=$(top_builddir)/util/src/libaxis2_util.la\
                                 $(top_builddir)/modules/core/transport/http/util/libaxis2_http_util.la

libaxis2_http_common_la_SOURCES = http_header.c\
                                  http_out_transport_info.c\
                                  http_request_line.c\
                                  http_simple_request.c\
                                  http_simple_response.c\
                                  http_status_line.c\
                                  http_worker.c


INCLUDES = -I$(top_builddir)/include \
	       -I$(top_builddir)/modules/core/transport\
	       -I$(top_builddir)/modules/core/transport/http \
           -I$(top_builddir)/modules/core/description \
           -I$(top_builddir)/modules/core/context \
           -I$(top_builddir)/modules/core/phaseresolver \
           -I$(top_builddir)/modules/core/engine \
           -I$(top_builddir)/modules/core/deployment \
           -I$(top_builddir)/util/include \
           -I$(top_builddir)/axiom/include
