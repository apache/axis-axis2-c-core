prgbindir=$(prefix)/bin/itest
prgbin_PROGRAMS = round1 
round1_SOURCES = round1_client.c

round1_LDADD   =    $(LDFLAGS) \
                    -laxis2_util \
					-laxis2_axiom \
					-laxis2_parser \
					$(top_builddir)/modules/core/engine/libaxis2_engine.la \
					$(top_builddir)/modules/wsdl/libaxis2_wsdl.la \
					$(top_builddir)//modules/core/transport/http/sender/libaxis2_http_sender.la \
					$(top_builddir)/modules/core/transport/http/receiver/libaxis2_http_receiver.la \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)

INCLUDES = -I$(top_builddir)/include \
           -I$(top_builddir)/xml_schema/include \
			@AXIOMINC@ \
			@UTILINC@ 

