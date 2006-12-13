prgbindir=$(prefix)/bin/samples
samplesdir=$(prefix)/samples/client/google
prgbin_PROGRAMS = yahoosearch
samples_DATA=yahoo_client.c Makefile.am Makefile.in
yahoosearch_SOURCES = yahoo_client.c

yahoosearch_LDADD   =    $(LDFLAGS) \
                    -L$(AXIS2C_HOME)/lib \
					-laxis2_util \
                    -laxis2_axiom \
                    -laxis2_wsdl \
                    -laxis2_engine \
                    -laxis2_parser \
                    -laxis2_http_sender \
                    -laxis2_http_receiver \
					-lwoden \
					-laxis2_xml_schema \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)

INCLUDES = -I$(AXIS2C_HOME)/include \
			@UTILINC@ \
			@AXIOMINC@

