prgbindir=$(prefix)/bin/samples
prgbin_PROGRAMS = diclient
samplesdir=$(prefix)/samples/client/diclient
samples_DATA=calc_client.c README Makefile.am Makefile.in
diclient_SOURCES = calc_client.c

diclient_LDADD   = $(LDFLAGS) \
                    -L$(AXIS2C_HOME)/lib \
					-laxis2_util \
                    -laxis2_axiom \
                    -laxis2_wsdl \
                    -laxis2_engine \
                    -laxis2_parser \
                    -lpthread \
					-lwoden \
					-laxis2_xml_schema \
                    -laxis2_http_sender \
                    -laxis2_http_receiver \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)

INCLUDES = -I$(AXIS2C_HOME)/include \
			@UTILINC@ \
			@AXIOMINC@
