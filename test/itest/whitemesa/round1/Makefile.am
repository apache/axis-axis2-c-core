prgbindir=$(prefix)/bin/itest
prgbin_PROGRAMS = round1 
round1_SOURCES = round1_client.c

round1_LDADD   =    $(LDFLAGS) \
                    $(top_builddir)/modules/util/libaxis2_util.la \
					$(top_builddir)/modules/platforms/unix/libaxis2_unix.la \
					$(top_builddir)/modules/xml/om/libaxis2_om.la \
					$(top_builddir)/modules/xml/soap/libaxis2_soap.la \
					$(top_builddir)/modules/core/engine/libaxis2_engine.la \
					$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
					$(top_builddir)/modules/xml/parser/libxml2/libaxis2_parser.la \
					$(top_builddir)//modules/core/transport/http/sender/libaxis2_http_sender.la \
					$(top_builddir)/modules/core/transport/http/receiver/libaxis2_http_receiver.la \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/modules/util \
            -I$(top_builddir)/modules/platforms \
            -I$(top_builddir)/modules/xml/parser


