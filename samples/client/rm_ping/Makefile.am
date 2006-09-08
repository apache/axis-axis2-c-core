prgbindir=$(prefix)/bin/samples
prgbin_PROGRAMS = rm_ping
samplesdir=$(prefix)/samples/client/rm_ping
samples_DATA=rm_ping_client.c Makefile.am Makefile.in
rm_ping_SOURCES = rm_ping_client.c

rm_ping_LDADD   =  \
                    -L$(AXIS2C_HOME)/lib \
					-laxis2_util \
                    -laxis2_axiom \
                    -laxis2_wsdl \
                    -laxis2_engine \
                    -laxis2_parser \
                    -lpthread \
                    -laxis2_http_sender \
                    -laxis2_http_receiver \
					-lwoden \
					-laxis2_xml_schema \
                    $(GUTHTHILA_LIBS) \
                    $(LIBXML2_LIBS)

INCLUDES = -I$(AXIS2C_HOME)/include \
			@UTILINC@ \
			@AXIOMINC@
