lib_LTLIBRARIES = libaxis2_http_receiver.la
libaxis2_http_receiver_la_LIBADD=	$(top_builddir)/util/src/libaxis2_util.la


libaxis2_http_receiver_la_SOURCES = simple_http_svr_conn.c\
				                    http_response_writer.c\
                                    http_svr_thread.c\
                                    http_server.c

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
