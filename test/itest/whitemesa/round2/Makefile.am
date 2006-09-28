prgbindir=$(prefix)/bin/itest
prgbin_PROGRAMS = round2 
round2_SOURCES = round2_client.c

round2_LDADD   =    $(LDFLAGS) \
                    ../../../../util/src/libaxis2_util.la \
                    ../../../../axiom/src/om/libaxis2_axiom.la \
                    ../../../../axiom/src/parser/$(PARSER_DIR)/libaxis2_parser.la \
                    ../../../../woden/src/wsdl20/libwoden.la \
                    ../../../../xml_schema/src/libaxis2_xml_schema.la \
					$(top_builddir)/modules/core/engine/libaxis2_engine.la \
					$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
					$(top_builddir)//modules/core/transport/http/sender/libaxis2_http_sender.la \
					$(top_builddir)/modules/core/transport/http/receiver/libaxis2_http_receiver.la \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)

INCLUDES = -I$(top_builddir)/include \
           -I$(top_builddir)/xml_schema/include \
           -I ../../../../util/include \
            -I ../../../../axiom/include

