lib_LTLIBRARIES = libaxis2_http_receiver.la
AM_CPPFLAGS = $(CPPFLAGS)
libaxis2_http_receiver_la_SOURCES = simple_http_svr_conn.c\
				                    http_response_writer.c\
                                    http_svr_thread.c\
                                    http_server.c

INCLUDES = -I$(top_builddir)/include \
           -I$(top_builddir)/modules/util \
           -I$(top_builddir)/modules/xml/parser\
	       -I$(top_builddir)/modules/wsdl\
	       -I$(top_builddir)/modules/core/transport\
	       -I$(top_builddir)/modules/core/transport/http \
           -I$(top_builddir)/modules/core/description \
           -I$(top_builddir)/modules/core/deployment \
           -I$(top_builddir)/modules/platforms
